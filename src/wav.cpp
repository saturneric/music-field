#include <wav.hpp>

extern WAV twav;

FILE * readWAVInfo(void){
    FILE *fp = NULL;
    struct WAV_Format wav;
    fp = fopen("test.wav", "rb");
    fread(&wav, 1, sizeof(struct WAV_Format), fp);
    
    return fp;
}


int init_test_wav(int ms) {
    twav.getInfo();
    twav.setTimer(ms);
    return sqrt(twav.m_pss.step);
}
