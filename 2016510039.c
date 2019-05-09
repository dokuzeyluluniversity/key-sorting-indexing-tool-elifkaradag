#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include<json-c/json.h>

    struct json_object *parsed_json;
	struct json_object *FileName;
	struct json_object *indexName;
	struct json_object *recordLength;
	struct json_object *keyEncoding;
	struct json_object *keyStart;
    struct json_object *keyEnd;
    struct json_object *order;

int main(int argc, char **argv) {
	FILE *fp;
	char buffer[1024];
   

	size_t i;	

	fp = fopen("j.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

	parsed_json = json_tokener_parse(buffer);

	json_object_object_get_ex(parsed_json, "dataFileName", &FileName);
	json_object_object_get_ex(parsed_json, "indexFileName", &indexName);
	json_object_object_get_ex(parsed_json, "recordLength", &recordLength);
    json_object_object_get_ex(parsed_json, "keyEncoding", &keyEncoding);
    json_object_object_get_ex(parsed_json, "recordLength", &recordLength);
    json_object_object_get_ex(parsed_json, "keyStart", &keyStart);
    json_object_object_get_ex(parsed_json, "keyEnd", &keyEnd);
    json_object_object_get_ex(parsed_json, "order", &order);

	printf("Name: %s\n", json_object_get_string(FileName));
	printf("Age: %d\n", json_object_get_int(keyStart));


}