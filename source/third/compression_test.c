#include "quicklz/quicklz.h"
#include "minilzo-2.10/minilzo.h"
#include "heatshrink/heatshrink_encoder.h"
#include "heatshrink/heatshrink_decoder.h"
#include "compression_test.h"

#include <stdio.h>



#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE (INPUT_BUFFER_SIZE + QLZ_BUFFER_PADDING)

const char *input_file_path = "input.txt";
const char *compressed_file_path = "compressed.qlz";
const char *decompressed_file_path = "decompressed.txt";

void QlzTest(void)
{
    // 初始化压缩和解压缩状态
    qlz_state_compress state_compress;
    qlz_state_decompress state_decompress;

    // 初始化压缩和解压缩缓冲区
    char compressed_data[OUTPUT_BUFFER_SIZE];
    char decompressed_data[INPUT_BUFFER_SIZE];
    
    // 打开输入文件
    FILE *input_file = fopen(input_file_path, "rb");
    if (!input_file) {
        perror("Failed to open input file");
        return 1;
    }

     // 读取输入文件内容
    char input_data[INPUT_BUFFER_SIZE];
    size_t input_size = fread(input_data, 1, INPUT_BUFFER_SIZE, input_file);
    fclose(input_file);

    // 压缩数据
    size_t compressed_size = qlz_compress(input_data, compressed_data, input_size, &state_compress);
    printf("Compressed size: %zu\n", compressed_size);

    // 打开压缩文件
    FILE *compressed_file = fopen(compressed_file_path, "wb");
    if (!compressed_file) {
        perror("Failed to open compressed file");
        return 1;
    }

    // 写入压缩数据到文件
    fwrite(compressed_data, 1, compressed_size, compressed_file);
    fclose(compressed_file);

    // 打开压缩文件以进行解压缩
    compressed_file = fopen(compressed_file_path, "rb");
    if (!compressed_file) {
        perror("Failed to open compressed file for reading");
        return 1;
    }

    // 读取压缩文件内容
    compressed_size = fread(compressed_data, 1, OUTPUT_BUFFER_SIZE, compressed_file);
    fclose(compressed_file);

    // 解压缩数据
    size_t decompressed_size = qlz_decompress(compressed_data, decompressed_data, &state_decompress);
    printf("Decompressed size: %zu\n", decompressed_size);
    
    // 打开解压缩文件
    FILE *decompressed_file = fopen(decompressed_file_path, "wb");
    if (!decompressed_file) {
        perror("Failed to open decompressed file");
        return 1;
    }

    // 写入解压缩数据到文件
    fwrite(decompressed_data, 1, decompressed_size, decompressed_file);
    fclose(decompressed_file);

    printf("File compression and decompression completed successfully.\n");

}