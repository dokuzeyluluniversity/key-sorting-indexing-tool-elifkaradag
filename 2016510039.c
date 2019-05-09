#include <json/json.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
struct _Json {
	char *datadat;
    char *indexndx;
	int reclength;
    char *keyEncoding;
    int keyStart;
    int keyEnd;
    char *order;	
};
typedef struct _Json JsonOBJ;
 
void json_parse(json_object * jobj) {
    
    //JsonOBJ jsonobj;
    int i=0;
    char array[7][20];
    void *ptr;
    enum json_type type;
    json_object_object_foreach(jobj, key, val) {
    type = json_object_get_type(val);
    switch (type) {
        case  json_type_string:
            printf("type: json_type_string, ");
            printf("value: %s\n", json_object_get_string(val));
            
           // array[i]=val;
            i++;
        break;
		case json_type_int:
		    printf("type: json_type_int, ");
            printf("value: %d\n", json_object_get_int(val));
		break;
        case json_type_boolean:
		    printf("type: json_type_boolean, ");
            printf("value: %b\n", json_object_get_int(val));
		break;
        case json_type_double:
		    printf("type: json_type_double, ");
            printf("value: %f\n", json_object_get_double(val));
		break;
        case json_type_array:
		    printf("type: json_type_array, ");
            printf("value: %s\n", json_object_get_array(val));
		break;
        case json_type_object:
		    printf("type: json_type_object, ");
            printf("value: %s\n", json_object_get_object(val));
		break;


        }
    }
}

int main() {
    char * string = "{ \"dataFileName\" : \"data.dat\", \"indexFileName\" : \"data.ndx\", \"recordLength\" : 401, \"keyEncoding\" : \"UTF\", \"keyStart\" : 13, \"keyEnd\" : 23, \"order\" : \"ACS\" }";
    printf ("JSON string: %s\n", string);
    json_object * jobj = json_tokener_parse(string);
    json_parse(jobj);
    return 0;
}