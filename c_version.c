#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include<string.h>





void main_work_two(FILE * file_ascii,size_t x,uint32_t wid,uint8_t r,uint8_t g,uint8_t b){

char darkness_scale[] = {' ', '.', '-', ',', '~', ':', ';', '=', '!', '*'
, '#', '%', 'i', 'j', 'L', '8', '&', 'Q', 'M', 'W', 'B', 'O', 'X','D','N', '@'};
// Calculate perceived brightness using a weighted average
int brightness = (0.299 * r + 0.587 * g + 0.114 * b);

int mix = brightness * (sizeof(darkness_scale) - 1) / 255;  // Scale brightness to the size of darkness_scale
// Ensure it's within the valid range

mix = (mix < 0) ? 0 : (mix > sizeof(darkness_scale) - 1) ? sizeof(darkness_scale) - 1 : mix;


if (x != wid - 1)
{
    fprintf(file_ascii,"%c",darkness_scale[mix]);
}else
{
    fprintf(file_ascii,"%c\n",darkness_scale[mix]);
}


}





void filp(uint32_t height,uint32_t width){

FILE *file_ascii;

file_ascii = fopen( "img_but_in_ascii_form.txt" , "r" );

FILE *file_ascii2;

file_ascii2 = fopen( "img_but_in_ascii_form2.txt" , "w" );

// Create an array to store each line
    char **lines = malloc(height * sizeof(char *));
    if (lines == NULL) {
        printf("Memory allocation error.\n");
        fclose(file_ascii);
        fclose(file_ascii2);
        return;
    }

    // Read each line and store it
    for (size_t i = 0; i < height; i++) {
        lines[i] = malloc((width + 2) * sizeof(char));  // +2 for newline and null terminator
        fgets(lines[i], width + 2, file_ascii);  // Read the line including the newline
    }

    // Write the lines in reverse order
    for (int i = height - 1; i >= 0; i--) {
        fputs(lines[i], file_ascii2);
        free(lines[i]);  // Free memory for each line after writing
    }

    free(lines);  // Free the array of lines

    fclose(file_ascii);
    fclose(file_ascii2);

}




void img(char *path){

FILE *file_ascii;

file_ascii = fopen( "img_but_in_ascii_form.txt" , "w" );

// reading the photo ? : my idea is using reding as binary and read every character

FILE *img_file;
img_file = fopen(path,"rb") ;

// test file is open


if (img_file == NULL)
{
    printf("img_file not read redo it?");
    return;
}



// read 14 bit file header and then read 40 bit info header


uint8_t file_header[14]; //you can use unsigne char[14] but i want because I'm profetionel 
size_t test_size =  fread(file_header,sizeof(uint8_t),14,img_file) ;

// test read using

if (test_size != 14)
{
    printf("its not 14?");
    return;
}


uint8_t file_info[40]; //you can use unsigne char[40] but i want because I'm profetionel 
test_size =  fread(file_info,sizeof(uint8_t),40,img_file) ;

// test read using

if (test_size != 40)
{
    printf("its not 40?");
    return;
}

uint16_t file_type = ( file_header[0] | file_header [1] << 8);
uint32_t file_size = ( file_header[2] | file_header[3] << 8 | file_header[4] << 16 | file_header[5] << 24 );
uint32_t offset_data = ( file_header[10] | file_header[11] << 8 | file_header[12] << 16 | file_header[13] << 24 ) ;


uint32_t width = ( file_info[4] | file_info[5] << 8 | file_info[6] << 16 | file_info[7] << 24 ) ;
uint32_t height = ( file_info[8] | file_info[9] << 8 | file_info[10] << 16 | file_info[11] << 24 );
uint16_t bit_count = ( file_info[14] | file_info[15] << 8  ) ;

printf(" %d %d %d %d %d %d \n",file_type,file_size,offset_data,width,height,bit_count);


// Ensure it's a valid BMP file

    if(file_type != 0x4D42){
        printf("Error: Not a valid BMP file.");
        return;
    }

// Ensure it is a 24-bit BMP file
    if (bit_count != 24){
        printf("Error: Only 24-bit BMP files are supported.");
        return;
    }

// ftell tell you were you are in  the fill
printf("%d\n" , ftell(img_file));

// Seek to the beginning of pixel data 
fseek(img_file,offset_data,SEEK_SET);

printf("%d\n" , ftell(img_file));

//Calculate row padding (each row must be aligned to a multiple of 4 bytes)
int row_padding = ( 4 - ( width * 3 ) % 4 ) % 4 ;

printf("%d\n" , row_padding);

uint8_t blue , green , red ;

// pixel loop

for (size_t y = 0; y < height; y++)
{
    uint8_t row [width*3] ;  // you can also do this  uint8_t pixel[3] Array to hold RGB values
    fread( row ,sizeof(uint8_t),width*3,img_file);
    for (size_t x = 0; x < width; x++)
    {
            blue = row[x*3];
            green = row[x*3+1];//fread(pixel, sizeof(uint8_t), 3, img_file);  Read RGB values
            red = row[x*3+2];//fprintf(file_ascii, "%d %d %d\n", pixel[2], pixel[1], pixel[0]);  Write to ASCII file (BGR to RGB)
            // printf("%d %d %d \n",red,green,blue);
            main_work_two(file_ascii,x,width,red,green,blue);
    }
    fseek(img_file,row_padding,SEEK_CUR);
    printf("The file is in part:  %.2f %% \n" , (float)(y*100)/height) ;
}

// ......................................... TEST ....................................................




// uint8_t pixel[3]; // Array to hold RGB values

// for (size_t y = 0; y < height; y++) {
//     for (size_t x = 0; x < width; x++) {
//         fread(pixel, sizeof(uint8_t), 3, img_file); // Read RGB values
//         printf("%d %d %d\n", pixel[2], pixel[1], pixel[0]); // Write to ASCII file (BGR to RGB
//     }
//     fseek(img_file, row_padding, SEEK_CUR); // Skip row padding
// }



//......................................... Finish TEST ....................................................

fclose(file_ascii);

filp(height,width);




fclose(img_file);

}
void main(){

    img("images.bmp");

}