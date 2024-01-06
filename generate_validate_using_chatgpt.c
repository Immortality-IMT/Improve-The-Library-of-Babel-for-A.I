/* 
   General GPT interface, prompt and return 0 or 1
   0 = adds to database
   1 = skip entery to database
*/

#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define API_KEY "YOUR_API_KEY"
#define MODEL_ID "YOUR_MODEL_ID"

// Callback function to handle HTTP response
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Process the response here if needed
    printf("Response: %.*s\n", (int)(size * nmemb), (char *)contents);

    // Placeholder logic to determine if the response corresponds to 0 or 1
    int isTrue = 1; // Replace with your logic
    if (isTrue) {
        // Do something for true case
        printf("Result is true.\n");
    } else {
        // Do something for false case
        printf("Result is false.\n");
    }

    return size * nmemb;
}

int main(void) {
    CURL *curl;
    CURLcode res;

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Set the API endpoint URL
        char url[256];
        snprintf(url, sizeof(url), "https://api.openai.com/v1/models/%s/completions", MODEL_ID);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the API key in the headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Bearer " API_KEY);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the prompt text to guide GPT-3.5 to output 0 or 1
        const char *prompt = "Return 0 or 1: ";

        // Create the JSON payload
        char payload[512];
        snprintf(payload, sizeof(payload), "{\"prompt\": \"%s\"}", prompt);

        // Set the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);

        // Set callback function to handle the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the HTTP POST request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Clean up
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

    return 0;
}
