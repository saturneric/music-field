#include <wav.hpp>

FILE * readWAVInfo(void){
    FILE *fp = NULL;
    struct WAV_Format wav;
    fp = fopen("test.wav", "rb");
    fread(&wav, 1, sizeof(struct WAV_Format), fp);
    
    return fp;
}
