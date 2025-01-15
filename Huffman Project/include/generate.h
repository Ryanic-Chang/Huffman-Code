#ifndef GENERATE_H
#define GENERATE_H

#include <stddef.h>
#include <json-c/json.h>

// Memory structure
struct Memory {
    char *response;
    size_t size;
};

// API constants
#define API_KEY "sk-fmauzpyfvydhkcealceosckekweqrwthkfbkyzxshjuhzrpa"
#define TEXT_GEN_URL "https://api.siliconflow.cn/v1/chat/completions"
#define IMAGE_GEN_URL "https://api.siliconflow.cn/v1/images/generations"
#define VIDEO_GEN_URL "https://api.siliconflow.cn/v1/video/submit"
#define VIDEO_STATUS_URL "https://api.siliconflow.cn/v1/video/status"

// Function declarations
size_t WriteCallback(void *data, size_t size, size_t nmemb, void *userp);
char *read_file(const char *filename);
void save_to_file(const char *filename, const char *content);
void download_file(const char *url, const char *output_filename);
void download_image(const char *url, const char *output_filename);
char *post_request(const char *url, const char *json_data);
void extract_and_save_text(const char *response, const char *output_file);
void download_video(const char *url, const char *output_filename);
void video_generation(const char *input_file);
void text_generation(const char *input_file);
void image_generation(const char *input_file);
void extract_text(const char *response);
char *request_chat(const char *url, struct json_object *messages);
void chat(const char *input_file);

#endif // GENERATE_H
