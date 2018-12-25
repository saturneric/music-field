#ifndef wav_hpp
#define wav_hpp

#include <type.hpp>
#include <graphs.hpp>

using std::string;
using std::vector;

FILE * readWAVInfo(void);

struct WAV_Format{
    uint32_t ChunkID;    /* "RIFF" */
    uint32_t ChunkSize;    /* 36 + Subchunk2Size */
    uint32_t Format; /* "WAVE" */
};

struct WAV_FMT {
    /* "fmt " */
    //uint32_t Subchunk1ID;
    /* 16 for PCM */
    //uint32_t Subchunk1Size;
    uint16_t AudioFormat;    /* PCM = 1*/
    uint16_t NumChannels;    /* Mono = 1, Stereo = 2, etc. */
    uint32_t SampleRate;    /* 8000, 44100, etc. */
    uint32_t ByteRate;    /* = SampleRate * NumChannels * BitsPerSample/8 */
    uint16_t BlockAlign;    /* = NumChannels * BitsPerSample/8 */
    uint16_t BitsPerSample;    /* 8bits, 16bits, etc. */
};

struct WAV_CHUNK {
    uint32_t SubchunkExID;
    uint32_t SubchunkExSize;
    uint32_t NumChannels;
};

struct CHUNK_INFO{
    uint32_t ChunkID;
    uint32_t ChunkSize;
};

struct WAV_DATA{
    /* sub-chunk "data" */
    uint32_t Subchunk2ID;    /* "data" */
    uint32_t Subchunk2Size;    /* data size */
};

struct process{
    uint32_t start,now,end,step;
};

struct Bit32
{
    unsigned short da:16;
    unsigned short db:16;
};

struct BitO16
{
    unsigned short da:16;
};

struct Bit16
{
    unsigned short da:8;
    unsigned short db:8;
};

struct BitO8
{
    unsigned short da:8;
};

class Data32{
public:
    bool active;
    vector<struct Bit32> bit;
    Data32(uint32_t *start, uint32_t *end):active(true){

        for(auto i = start;i != end; i++){
            struct Bit32 temp;
            memcpy(&temp, i, sizeof(struct Bit32));
            bit.push_back(temp);
        }
    }
    Data32(const Data32 &tmp){
        active = tmp.active;
        bit = tmp.bit;
    }
    
};

class Data{
public:
    bool active;
    uint32_t channels, bit_size;
    vector<struct Bit32> bit32;
    vector<struct BitO16> bito16;
    vector<struct Bit16> bit16;
    vector<struct BitO8> bito8;
    vector<Color> color_data;
    Color avg_color;
    Data(uint32_t tchannels, uint32_t tbit_size,void *start, void *end):active(true),channels(tchannels),bit_size(tbit_size){
        if (bit_size == 32){
            uint32_t *m_s = (uint32_t *)start, *m_e = (uint32_t *)end;
            for(auto i = m_s;i != m_e; i++){
                struct Bit32 temp;
                memcpy(&temp, i, sizeof(struct Bit32));
                bit32.push_back(temp);
            }
        }
        else if(bit_size == 16){
            uint16_t *m_s = (uint16_t *)start, *m_e = (uint16_t *)end;
            if(channels == 2)
                for(auto i = m_s;i != m_e; i++){
                    struct Bit16 temp;
                    memcpy(&temp, i, sizeof(struct Bit16));
                    bit16.push_back(temp);
                }
            else{
                for(auto i = m_s;i != m_e; i++){
                    struct BitO16 temp;
                    memcpy(&temp, i, sizeof(struct BitO16));
                    bito16.push_back(temp);
                }
            }
        }
        else if(bit_size == 8){
            uint8_t *m_s = (uint8_t *)start, *m_e = (uint8_t *)end;
            for(auto i = m_s;i != m_e; i++){
                struct BitO8 temp;
                memcpy(&temp, i, sizeof(struct BitO8));
                bito8.push_back(temp);
            }
        }
    }
    Data(const Data &tmp){
        bit_size = tmp.bit_size;
        channels = tmp.channels;
        active = tmp.active;
        bit32 = tmp.bit32;
        bito16 = tmp.bito16;
        bit16 = tmp.bit16;
        bito8 = tmp.bito8;
    }
    void Data2Color(void){
        if(channels == 2){
            if(bit_size == 32){
                double sumr = 0 ,sumb = 0, sumg = 0;
                //printf("BLOCKS\n");
                for(auto i = bit32.begin(); i != bit32.end(); i++){
                    Bit32 bit = (*i);
                    if(i != bit32.begin()){
                        Color m_color;
                        
                        //printf("%d %d\n",bit.da,bit.db);
                        double cb = log2(sqrt(bit.da + bit.db)+1)*18.5, cg = log2(sqrt(abs(bit.db - (*(i-1)).db))+1)*42, cr = log2(sqrt(abs(bit.da - (*(i-1)).da))+1)*42;
                        cr = cr>255?255:cr;
                        cg = cg>255?255:cg;
                        cb = cb>255?255:cb;
                        sumr += cr;
                        sumb += cb;
                        sumg += cg;
                        m_color.setColor({cr,cb,cg});
                        color_data.push_back(m_color);
                    }
                }
                sumr/=(bit32.size()-1);
                sumb/=(bit32.size()-1);
                sumg/=(bit32.size()-1);
                avg_color.setColor({sumr,sumb,sumg});
            }
            else{
                double sumr = 0 ,sumb = 0, sumg = 0;
                for(auto i = bit16.begin(); i != bit16.end(); i++){
                    Bit16 bit = (*i);
                    if(i != bit16.begin()){
                        Color m_color;
                        double cb = log2(bit.da + bit.db + 1)*18.5, cg = log2(abs(bit.db - (*(i-1)).db)+1)*22.5, cr = log2(abs(bit.da - (*(i-1)).da)+1)*22.5;
                        cr = cr>255?255:cr;
                        cg = cg>255?255:cg;
                        cb = cb>255?255:cb;
                        sumr += cr;
                        sumb += cb;
                        sumg += cg;
                        m_color.setColor({cr,cb,cg});
                        color_data.push_back(m_color);
                    }
                }
                avg_color.setColor({sumr,sumb,sumg});
            }
        }
        else{
            if(bit_size == 16){
                double sumr = 0 ,sumg = 0;
                for(auto i = bito16.begin(); i != bito16.end(); i++){
                    BitO16 bit = (*i);
                    if(i != bito16.begin()){
                        Color m_color;
                        double cg = log2(bit.da + 1)*18.5, cr = log2(abs(bit.da - (*(i-1)).da)+1)*22.5;
                        cr = cr>255?255:cr;
                        cg = cg>255?255:cg;
                        sumr += cr;
                        sumg += cg;
                        m_color.setColor({cr,cg,0});
                        color_data.push_back(m_color);
                    }
                }
                avg_color.setColor({sumr,sumg,0});
            }
            else{
                double sumr = 0 ,sumb = 0;
                for(auto i = bito8.begin(); i != bito8.end(); i++){
                    BitO8 bit = (*i);
                    if(i != bito8.begin()){
                        Color m_color;
                        double cb = log2(bit.da + 1)*18.5, cr = log2(abs(bit.da - (*(i-1)).da)+1)*22.5;
                        cr = cr>255?255:cr;
                        cb = cb>255?255:cb;
                        sumr += cr;
                        sumb += cb;
                        m_color.setColor({cr,cb,0});
                        color_data.push_back(m_color);
                    }
                }
                avg_color.setColor({sumr,sumb,0});
            }
        }
    }
};

class WAV{
public:
    struct WAV_Format format;
    struct WAV_FMT fmt;
    struct WAV_DATA data;
    struct WAV_CHUNK fact;
    struct WAV_CHUNK list;
    struct CHUNK_INFO info;
    uint32_t file_size;
    bool active,if_fmt;
    FILE *fp;
    vector<uint32_t> data32;
    vector<uint16_t> data16;
    vector<uint8_t> data8;
    uint32_t seconds;
    struct process m_pss;
    WAV(string path):active(false),if_fmt(false){
        fp = fopen(path.data(), "rb");
        if(fp == NULL) throw -1;
        
        do{
            fread(&this->info, 1, sizeof(struct CHUNK_INFO), fp);
            char *p_id = (char *)(&info.ChunkID);
            
            uint32_t temp;
            char *p_tmp = (char *)(&temp);
            if(*p_id=='R' && *(p_id+1)=='I' && *(p_id+2)=='F' && *(p_id+3)=='F'){
                format.ChunkID = info.ChunkID;
                format.ChunkSize = info.ChunkSize;
                file_size = info.ChunkSize+8;
                fread(&temp, 1, sizeof(uint32_t), fp);
                if(*p_tmp=='W' && *(p_tmp+1)=='A' && *(p_tmp+2)=='V' && *(p_tmp+3)=='E'){
                    active = true;
                    format.Format = temp;
                }
                else{
                    active = false;
                    throw -1;
                }
                
            }
            else if(*p_id=='L' && *(p_id+1)=='I' && *(p_id+2)=='S' && *(p_id+3)=='T'){
                fseek(fp, info.ChunkSize, SEEK_CUR);
            }
            else if(*p_id=='f' && *(p_id+1)=='m' && *(p_id+2)=='t'){
                if_fmt = true;
                fread(&this->fmt, 1, sizeof(WAV_FMT), fp);
            }
            else if(*p_id=='f' && *(p_id+1)=='a' && *(p_id+2)=='c' && *(p_id+3)=='t'){
                fseek(fp, info.ChunkSize, SEEK_CUR);
            }
            else if(*p_id=='d' && *(p_id+1)=='a' && *(p_id+2)=='t' && *(p_id+3)=='a'){
                if(if_fmt == false) throw -1;
                else if(fmt.AudioFormat != 1) throw -1;
                else{
                    data.Subchunk2ID = info.ChunkID;
                    data.Subchunk2Size = info.ChunkSize;
                    break;
                }
            }
            
        }while(!feof(fp) && active == true);
        /*fread (&this->format, 1, sizeof(struct WAV_Format), fp);
        fread (&this->fmt, 1, sizeof(struct WAV_FMT), fp);
        char if_fact[5];
        do{
            fread (&if_fact, 1, sizeof(uint32_t), fp);
            if_fact[4] = '\0';
            if(!strcmp(if_fact, "fact"))
                fread(&this->fact, 1, sizeof(struct WAV_CHUNK), fp);
            else if (!strcmp(if_fact, "LIST"))
                fread(&this->list, 1, sizeof(struct WAV_CHUNK), fp);
            fseek(fp, -sizeof(uint32_t), SEEK_CUR);
            
        }while(strcmp(if_fact, "data"));
        fread(&this->data, 1, sizeof(struct WAV_DATA), fp);*/
        if(fmt.BlockAlign == 4){
            uint32_t t_data32, count = data.Subchunk2Size/sizeof(uint32_t);
            while(fread(&t_data32, 1, sizeof(uint32_t), fp) && ~count){
                data32.push_back(t_data32);
                count--;
            }
        }
        else if(fmt.BlockAlign == 2){
            uint16_t t_data16;
            uint32_t count = data.Subchunk2Size/sizeof(uint16_t);
            while(fread(&t_data16, 1, sizeof(uint16_t), fp) && ~count){
                data16.push_back(t_data16);
                count--;
            }
        }
        else if(fmt.BlockAlign == 1){
            uint8_t t_data8;
            uint32_t count = data.Subchunk2Size/sizeof(uint8_t);
            while(fread(&t_data8, 1, sizeof(uint8_t), fp) && ~count){
                data8.push_back(t_data8);
                count--;
            }
        }
        seconds = data.Subchunk2Size/fmt.ByteRate;
    }
    void setTimer(int ms){
        if(ms <= 1000){
            m_pss.step = (((double)ms/seconds)*((data.Subchunk2Size/1000.0)/(double)fmt.BlockAlign));
            m_pss.end = data.Subchunk2Size/(double)fmt.BlockAlign;
            m_pss.start = 0;
            m_pss.now = 0;
        }
    }
    
    Data32 getData32(void){
        if(m_pss.now < m_pss.end && m_pss.now + m_pss.step < m_pss.end){
            Data32 m_data(&(data32[m_pss.now]),
                          &(data32[m_pss.now + m_pss.step]));
            m_pss.now += m_pss.step;
            return m_data;
        }
        else{
            Data32 m_data(NULL,NULL);
            m_data.active = false;
            return m_data;
        }
    }
    
    Data getData(void){
        if(m_pss.now < m_pss.end && m_pss.now + m_pss.step < m_pss.end){
            if(fmt.NumChannels == 2){
                if(fmt.BlockAlign == 4){
                    Data m_data(fmt.NumChannels,fmt.BlockAlign*8,&data32[m_pss.now],
                                &data32[m_pss.now + m_pss.step]);
                    m_pss.now += m_pss.step;
                    return m_data;
                }
                else if(fmt.BlockAlign == 2){
                    Data m_data(fmt.NumChannels,fmt.BlockAlign*8,&data16[m_pss.now],
                                &data16[m_pss.now + m_pss.step]);
                    m_pss.now += m_pss.step;
                    return m_data;
                }
            }
            else{
                if(fmt.BlockAlign == 2){
                    Data m_data(fmt.NumChannels,fmt.BlockAlign*8,&data16[m_pss.now],
                                &data16[m_pss.now + m_pss.step]);
                    m_pss.now += m_pss.step;
                    return m_data;
                }
                else if(fmt.BlockAlign == 1){
                    Data m_data(fmt.NumChannels,fmt.BlockAlign*8,&data8[m_pss.now],
                                &data8[m_pss.now + m_pss.step]);
                    m_pss.now += m_pss.step;
                    return m_data;
                }
            }
        }
        Data m_data(0,0,NULL,NULL);
        m_data.active = false;
        return m_data;
    }
    
    void getInfo(void){
        printf("ChunkID \t%x\n", format.ChunkID);
        printf("ChunkSize \t%d\n", format.ChunkSize);
        printf("Format \t\t%d\n", format.Format);
        printf("AudioFormat \t%d\n", fmt.AudioFormat);
        printf("NumChannels \t%d\n", fmt.NumChannels);
        printf("SampleRate \t%d\n", fmt.SampleRate);
        printf("ByteRate \t%d\n", fmt.ByteRate);
        printf("BlockAlign \t%d\n", fmt.BlockAlign);
        printf("BitsPerSample \t%d\n", fmt.BitsPerSample);
        printf("Subchunk2ID \t%x\n", data.Subchunk2ID);
        printf("Subchunk2Size \t%d\n", data.Subchunk2Size);
    }
};



#endif /* wav_hpp */
