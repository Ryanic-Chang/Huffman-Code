#include "generate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>

size_t WriteCallback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (!ptr) {
        printf("Not enough memory!\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file"); 
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long filesize = ftell(file);
    rewind(file);

    char *content = (char *)malloc(filesize + 1);
    if (!content) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    fread(content, 1, filesize, file);
    content[filesize] = '\0';
    fclose(file);
    return content;
}

void save_to_file(const char *filename, const char *content) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }
    fprintf(file, "%s", content);
    fclose(file);
    printf("Result saved to %s\n", filename);
}

void download_file(const char *url, const char *output_filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(output_filename, "wb");
        if (!fp) {
            perror("Failed to open file for writing");
            curl_easy_cleanup(curl);
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "File download failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("File saved to %s\n", output_filename);
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }
}

void download_image(const char *url, const char *output_filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(output_filename, "wb");
        if (!fp) {
            perror("Failed to open file for writing image");
            curl_easy_cleanup(curl);
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Image download failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Image saved to %s\n", output_filename);
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }
}

char *post_request(const char *url, const char *json_data) {
    CURL *curl;
    CURLcode res;
    struct Memory chunk = {NULL, 0};

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return NULL;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);
    headers = curl_slist_append(headers, auth_header);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return chunk.response;
}

void extract_and_save_text(const char *response, const char *output_file) {
    struct json_object *parsed_json = json_tokener_parse(response);
    if (!parsed_json) {
        fprintf(stderr, "Failed to parse JSON response\n");
        return;
    }

    struct json_object *choices;
    if (json_object_object_get_ex(parsed_json, "choices", &choices)) {
        struct json_object *choice = json_object_array_get_idx(choices, 0);
        struct json_object *message;
        if (json_object_object_get_ex(choice, "message", &message)) {
            struct json_object *content;
            if (json_object_object_get_ex(message, "content", &content)) {
                const char *text = json_object_get_string(content);
                save_to_file(output_file, text);
            }
        }
    }

    json_object_put(parsed_json);
}

void download_video(const char *url, const char *output_filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(output_filename, "wb");
        if (!fp) {
            perror("Failed to open file for writing video");
            curl_easy_cleanup(curl);
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Video download failed: %s\n", curl_easy_strerror(res));
        } else {
            printf("Video saved to %s\n", output_filename);
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    }
}

void video_generation(const char *input_file) {
    char *prompt = read_file(input_file);
    if (!prompt) return;

    // 提交视频生成请求
    struct json_object *json_obj = json_object_new_object();
    json_object_object_add(json_obj, "model", json_object_new_string("Lightricks/LTX-Video"));
    json_object_object_add(json_obj, "prompt", json_object_new_string(prompt));
    //json_object_object_add(json_obj, "image", json_object_new_string(""));
    json_object_object_add(json_obj, "seed", json_object_new_int(123));

    char *json_data = strdup(json_object_to_json_string(json_obj));

    char *response = post_request(VIDEO_GEN_URL, json_data);
    if (!response) {
        fprintf(stderr, "Video generation request failed\n");
        free(prompt);
        free(json_data);
        json_object_put(json_obj);
        return;
    }

    struct json_object *parsed_json = json_tokener_parse(response);
    if (!parsed_json) {
        fprintf(stderr, "Failed to parse JSON response\n");
        free(response);
        free(prompt);
        free(json_data);
        json_object_put(json_obj);
        return;
    }

    struct json_object *request_id_obj;
    if (!json_object_object_get_ex(parsed_json, "requestId", &request_id_obj)) {
        fprintf(stderr, "No requestId found in response\n");
        json_object_put(parsed_json);
        free(response);
        free(prompt);
        free(json_data);
        json_object_put(json_obj);
        return;
    }

    const char *request_id = json_object_get_string(request_id_obj);
    printf("Video generation request submitted. Request ID: %s\n", request_id);

    // 查询视频状态
    struct json_object *status_obj = json_object_new_object();
    json_object_object_add(status_obj, "requestId", json_object_new_string(request_id));

    char *status_data = strdup(json_object_to_json_string(status_obj));
    char *status_response;
    int max_retries = 1000;
    int retries = 0;

    do {
        Sleep(5000); // 等待 5 秒
        status_response = post_request(VIDEO_STATUS_URL, status_data);
        if (!status_response) {
            fprintf(stderr, "Video status request failed\n");
            break;
        }

        struct json_object *status_json = json_tokener_parse(status_response);
        if (!status_json) {
            fprintf(stderr, "Failed to parse status JSON\n");
            free(status_response);
            break;
        }

        struct json_object *status_obj;
        if (json_object_object_get_ex(status_json, "status", &status_obj)) {
            const char *status = json_object_get_string(status_obj);

            if (strcmp(status, "Succeed") == 0) { // 修改为检查 "Succeed"
                struct json_object *videos_obj;
                if (json_object_object_get_ex(status_json, "results", &videos_obj)) {
                    struct json_object *videos_array;
                    if (json_object_object_get_ex(videos_obj, "videos", &videos_array)) {
                        struct json_object *first_video = json_object_array_get_idx(videos_array, 0);
                        if (first_video) {
                            struct json_object *url_obj;
                            if (json_object_object_get_ex(first_video, "url", &url_obj)) {
                                const char *video_url = json_object_get_string(url_obj);
                                printf("Video URL: %s\n", video_url);
                                download_video(video_url, "output/generated_video.mp4");
                            }
                        }
                    }
                }
                json_object_put(status_json); // 清理内存
                break;
            }
        }

        json_object_put(status_json); // 清理内存
        free(status_response); // 清理内存
        retries++;
    } while (retries < max_retries);

    // 清理内存
    free(prompt);
    free(json_data);
    free(status_data);
    json_object_put(json_obj);
}

void text_generation(const char *input_file) {
    char *text = read_file(input_file);
    if (!text) return;

    struct json_object *json_obj = json_object_new_object();
    struct json_object *messages = json_object_new_array();

    struct json_object *user_msg = json_object_new_object();
    json_object_object_add(user_msg, "role", json_object_new_string("user"));
    json_object_object_add(user_msg, "content", json_object_new_string(text));

    json_object_array_add(messages, user_msg);

    json_object_object_add(json_obj, "model", json_object_new_string("Qwen/Qwen2.5-7B-Instruct"));
    json_object_object_add(json_obj, "messages", messages);
    json_object_object_add(json_obj, "stream", json_object_new_boolean(0));
    json_object_object_add(json_obj, "max_tokens", json_object_new_int(4096));
    json_object_object_add(json_obj, "temperature", json_object_new_double(0.7));
    json_object_object_add(json_obj, "top_p", json_object_new_double(0.7));
    json_object_object_add(json_obj, "top_k", json_object_new_int(50));
    json_object_object_add(json_obj, "frequency_penalty", json_object_new_double(0.0));
    json_object_object_add(json_obj, "n", json_object_new_int(1));

    char *json_data = strdup(json_object_to_json_string(json_obj));
    char *response = post_request(TEXT_GEN_URL, json_data);
    if (response) {
        extract_and_save_text(response, "output/text_summary.md");
        free(response);
    }

    free(text);
    free(json_data);
    json_object_put(json_obj);
}

void image_generation(const char *input_file) {
    char *prompt = read_file(input_file);
    if (!prompt) return;

    struct json_object *json_obj = json_object_new_object();
    json_object_object_add(json_obj, "model", json_object_new_string("stabilityai/stable-diffusion-3-5-large"));
    json_object_object_add(json_obj, "prompt", json_object_new_string(prompt));
    json_object_object_add(json_obj, "image_size", json_object_new_string("1024x1024"));
    json_object_object_add(json_obj, "batch_size", json_object_new_int(1));
    json_object_object_add(json_obj, "num_inference_steps", json_object_new_int(20));
    json_object_object_add(json_obj, "guidance_scale", json_object_new_double(7.5));

    char *json_data = strdup(json_object_to_json_string(json_obj));
    char *response = post_request(IMAGE_GEN_URL, json_data);
    if (response) {
        struct json_object *parsed_json = json_tokener_parse(response);
        if (parsed_json) {
            struct json_object *data;
            if (json_object_object_get_ex(parsed_json, "data", &data)) {
                struct json_object *first_image = json_object_array_get_idx(data, 0);
                struct json_object *url;
                if (json_object_object_get_ex(first_image, "url", &url)) {
                    const char *image_url = json_object_get_string(url);
                    download_image(image_url, "output/generated_image.jpeg");
                }
            }
            json_object_put(parsed_json);
        }
        free(response);
    }

    free(prompt);
    free(json_data);
    json_object_put(json_obj);
}

// 解析 JSON 响应
void extract_text(const char *response) {
    // 解析 JSON 响应
    struct json_object *parsed_json = json_tokener_parse(response);
    if (!parsed_json) {
        fprintf(stderr, "Failed to parse JSON response\n");
        return;
    }

    struct json_object *choices;
    if (json_object_object_get_ex(parsed_json, "choices", &choices)) {
        // 获取第一个 choice 对象
        struct json_object *choice = json_object_array_get_idx(choices, 0);
        struct json_object *message;
        if (json_object_object_get_ex(choice, "message", &message)) {
            struct json_object *content;
            if (json_object_object_get_ex(message, "content", &content)) {
                // 获取 "content" 字段的字符串内容
                const char *text = json_object_get_string(content);
                printf("AI: %s\n", text);  // 打印 AI 回复内容
            }
        }
    }

    json_object_put(parsed_json);  // 释放 JSON 对象内存
}

char *request_chat(const char *url, struct json_object *messages) {
    CURL *curl;
    CURLcode res;
    struct Memory chunk = {NULL, 0};

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return NULL;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", API_KEY);
    headers = curl_slist_append(headers, auth_header);

    // 构造请求JSON
    struct json_object *json_obj = json_object_new_object();
    json_object_object_add(json_obj, "model", json_object_new_string("Qwen/Qwen2.5-7B-Instruct"));
    json_object_object_add(json_obj, "messages", messages);
    json_object_object_add(json_obj, "stream", json_object_new_boolean(0));
    json_object_object_add(json_obj, "max_tokens", json_object_new_int(4096));
    json_object_object_add(json_obj, "temperature", json_object_new_double(0.7));
    json_object_object_add(json_obj, "top_p", json_object_new_double(0.7));
    json_object_object_add(json_obj, "top_k", json_object_new_int(50));
    json_object_object_add(json_obj, "frequency_penalty", json_object_new_double(0.0));
    json_object_object_add(json_obj, "n", json_object_new_int(1));

    const char *json_data = json_object_to_json_string(json_obj);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // 设置超时机制
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60L);          // 总超时 60 秒
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);  // 连接超时 10 秒

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL request failed: %s\n", curl_easy_strerror(res));
    }

    // 释放JSON对象和CURL资源
    json_object_put(json_obj);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    return chunk.response;
}

void chat(const char *input_file) {
    // 读取初始文件内容
    char *text = read_file(input_file);
    if (!text) {
        fprintf(stderr, "Failed to load %s\n", input_file);
        return;
    }

    printf("\nUpload %s successfully.\n", input_file);

    while (1) {
        // 初始化消息数组，仅包含初始文件内容
        struct json_object *messages = json_object_new_array();

        // 添加初始用户消息
        struct json_object *user_msg = json_object_new_object();
        json_object_object_add(user_msg, "role", json_object_new_string("user"));
        json_object_object_add(user_msg, "content", json_object_new_string(text));
        json_object_array_add(messages, user_msg);

        // 获取用户输入
        char user_input[1024];
        printf("\nYou: ");
        if (!fgets(user_input, sizeof(user_input), stdin)) {
            perror("Failed to read input");
            break;
        }

        // 去掉换行符
        user_input[strcspn(user_input, "\n")] = '\0';

        // 检测退出指令
        if (strcmp(user_input, "exit") == 0) {
            printf("Exiting chat...\n");
            break;
        }

        // 将用户输入作为第二条消息添加到上下文
        user_msg = json_object_new_object();
        json_object_object_add(user_msg, "role", json_object_new_string("user"));
        json_object_object_add(user_msg, "content", json_object_new_string(user_input));
        json_object_array_add(messages, user_msg);

        // 发起请求
        char *response = request_chat(TEXT_GEN_URL, messages);

        if (response) {
            // 打印AI回复
            extract_text(response);
            free(response);
        } else {
            printf("AI: (No response, please try again later)\n");
        }

        // 释放当前消息上下文
        //json_object_put(messages);
    }

    // 释放初始文件内容
    free(text);
}