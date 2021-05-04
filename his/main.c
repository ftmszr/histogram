#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define B2L_ENDIAN(num) (((num >> 24) & 0xff) | ((num << 8) & 0xff0000) | ((num >> 8) & 0xff00) | ((num << 24) & 0xff000000))


extern int is_little_endian();
extern uint8_t* read_file(const char*, size_t*);
extern void write_file(const uint8_t*, const char*, size_t);
extern void histogram(const uint8_t*, size_t);

int main()
{
    uint8_t* buffer;
    uint32_t offset, en, boy1;
    size_t boy;

    buffer = read_file("deneme.bmp", &boy);

    if (is_little_endian()) {
        offset  = *((uint32_t*)(&buffer[10]));
        en   = *((uint32_t*)(&buffer[18]));
        boy1  = *((uint32_t*)(&buffer[22]));
    }
    else {
        offset  = B2L_ENDIAN(*((uint32_t*)(&buffer[10])));
        en   = B2L_ENDIAN(*((uint32_t*)(&buffer[18])));
        boy1  = B2L_ENDIAN(*((uint32_t*)(&buffer[22])));
    }

    histogram(&buffer[offset], en * boy1);


    return 0;
}

int is_little_endian() {
    uint16_t word = 0x0001;
    uint8_t* ptr = (uint8_t*)&word;
    return (ptr[0]) ? 1 : 0;
}

uint8_t* read_file(const char* file_path, size_t* boy) {
    FILE* fp = fopen(file_path, "rb");

    if (!fp) {
        fprintf(stderr, "Dosya bulunamadý\n");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    *boy = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    uint8_t* buffer = (uint8_t*)malloc(*boy);

    if (!buffer) {
        fprintf(stderr, "Buffer bos\n");
        exit(1);
    }

    fread(buffer, *boy, 1, fp);
    fclose(fp);
    return buffer;
}

void write_file(const uint8_t* buffer, const char* file_path, size_t boy) {
    FILE* fp = fopen(file_path, "wb");

    if (!fp) {
        fprintf(stderr, "Dosya yapýlamadý\n");
        exit(1);
    }

    fwrite(buffer, boy, 1, fp);
    fclose(fp);
}

void histogram(const uint8_t* buffer, size_t boy) {
    int i, bins[256] = { 0 };
    float ort, varyans;

    i=0;
    while ( i < boy )
    {
        bins[buffer[i]]++;
        i++;
    }
    printf("Histogram\n\n");

    i=0;
    while (i < 256)
    {
        printf("H(%03d) = %d\n", i, bins[i]);
        i++;
    }

    ort = 0.0f;

    i=0;
    while (i < 256)
    {
        ort += i * bins[i];
        i++;
    }
    ort = ort / boy;

    printf("\nortalama : %.2f\n", ort);

    varyans = 0.0f;

    i=0;
    while (i < 256)
    {
        varyans += (i - ort) * (i - ort) * bins[i];
        i++;
    }

    varyans = varyans / (boy - 1);

    printf("\nvaryans : %.2f\n", varyans);
}
