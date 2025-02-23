/**
 * @file cson.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief cson
 * @version 0.1
 * @date 2019-08-26
 * 
 * @copyright (c) 2019 Letter
 * 
 */

#include "cson.h"
#include "cJSON.h"
#include "stddef.h"
#include "string.h"
#include "stdio.h"

/**
 * @brief 基本类型链表数据模型
 * 
 */
CsonModel csonBasicListModel[] =
{
    [0] = {CSON_TYPE_OBJ, NULL, 0, .param.objSize=sizeof(char)},
    [1] = {CSON_TYPE_CHAR, NULL, 0},
    [2] = {CSON_TYPE_OBJ, NULL, 0, .param.objSize=sizeof(short)},
    [3] = {CSON_TYPE_SHORT, NULL, 0},
    [4] = {CSON_TYPE_OBJ, NULL, 0, .param.objSize=sizeof(int)},
    [5] = {CSON_TYPE_INT, NULL, 0},
    [6] = {CSON_TYPE_OBJ, NULL, 0, .param.objSize=sizeof(long)},
    [7] = {CSON_TYPE_LONG, NULL, 0},
    [8] = {CSON_TYPE_OBJ, NULL, 0, .param.objSize=sizeof(float)},
    [9] = {CSON_TYPE_FLOAT, NULL, 0},
    [10] = {CSON_TYPE_OBJ, NULL, 0, .param.objSize=sizeof(double)},
    [11] = {CSON_TYPE_DOUBLE, NULL, 0},
    [12] = {CSON_TYPE_OBJ, NULL, 0, .param.objSize=sizeof(char *)},
    [13] = {CSON_TYPE_STRING, NULL, 0},
};


/**
 * @brief cson
 * 
 */
struct {
    void *(*malloc)(int);
    void (*free)(void *);
} cson;


/**
 * @brief CSON初始化
 * 
 * @param malloc 内存分配函数
 * @param free 内存释放函数
 */
void csonInit(void *malloc, void *free)
{
    cson.malloc = (void *(*)(int))malloc;
    cson.free = (void (*)(void *))free;
    cJSON_InitHooks(&(cJSON_Hooks){(void *(*)(size_t))cson.malloc, cson.free});
}


signed char csonIsBasicListModel(CsonModel *model)
{
    return (model >= &csonBasicListModel[0] && model <= &csonBasicListModel[13]) ? 1 : 0;
}


/**
 * @brief 解析JSON整型
 * 
 * @param json JSON对象
 * @param key key
 * @return int 解析出的整型数
 */
int csonDecodeNumber(cJSON *json, char *key)
{
	printf("[%s]key:%s\n", __func__, key);
    cJSON *item = key ? cJSON_GetObjectItem(json, key) : json;
    if (item && item->type == cJSON_Number)
    {
        return item->valueint;
    }
    printf("[%s]no key found[%s]\n", __func__, key);
    return 0;
}

/**
 * @brief 解析JSON整型
 *
 * @param json JSON对象
 * @param key key
 * @return int 解析出的整型数
 */
int csonDecodeNumber2(cJSON *json, char *key, void* data,  CsonType type)
{
	printf("[%s]key:%s\n", __func__, key);
    cJSON *item = key ? cJSON_GetObjectItem(json, key) : json;
    if (item && item->type == cJSON_Number)
    {
    	switch (type)
		{
		case CSON_TYPE_CHAR:
			((z_char*)data)->data = (char)item->valueint;
			((z_char*)data)->flag = 1;
			break;
		case CSON_TYPE_SHORT:
			((z_short*)data)->data = (short)item->valueint;
			((z_short*)data)->flag = 1;
			break;
		case CSON_TYPE_INT:
			((z_int*)data)->data = (int)item->valueint;
			((z_int*)data)->flag = 1;
			break;
		case CSON_TYPE_LONG:
			((z_long*)data)->data = (int)item->valueint;
			((z_long*)data)->flag = 1;
			break;
		default:
			((z_int*)data)->data = (int)item->valueint;
			((z_int*)data)->flag = 1;
			break;
		}
    	return 0;
    }
    printf("[%s]no key found[%s]\n", __func__, key);
    switch (type)
	{
	case CSON_TYPE_CHAR:
		((z_char*)data)->data = 0;
		((z_char*)data)->flag = 0;
		break;
	case CSON_TYPE_SHORT:
		((z_short*)data)->data = 0;
		((z_short*)data)->flag = 1;
		break;
	case CSON_TYPE_INT:
		((z_int*)data)->data = 0;
		((z_int*)data)->flag = 1;
		break;
	case CSON_TYPE_LONG:
		((z_long*)data)->data = 0;
		((z_long*)data)->flag = 1;
		break;
	default:
		((z_int*)data)->data = 0;
		((z_int*)data)->flag = 1;
		break;
	}
    return -1;
}


/**
 * @brief 解析JSON浮点型
 * 
 * @param json JSON对象
 * @param key key
 * @return double 解析出的浮点型数
 */
double csonDecodeDouble(cJSON *json, char *key)
{
    cJSON *item = key ? cJSON_GetObjectItem(json, key) : json;
    if (item && item->type == cJSON_Number)
    {
        return item->valuedouble;
    }
    return 0.0;
}

/**
 * @brief 解析JSON浮点型
 *
 * @param json JSON对象
 * @param key key
 * @return double 解析出的浮点型数
 */
double csonDecodeDouble2(cJSON *json, char *key, void* data, CsonType type)
{
    cJSON *item = key ? cJSON_GetObjectItem(json, key) : json;
    if (item && item->type == cJSON_Number)
    {
    	switch(type)
    	{
    	 	case CSON_TYPE_FLOAT:
    	 		((z_float*)data)->data = (float)item->valuedouble;
    	 		((z_float*)data)->flag = 1;
    	 		break;
    	    case CSON_TYPE_DOUBLE:
    	    	((z_double*)data)->data = (float)item->valuedouble;
    	    	((z_double*)data)->flag = 1;
    	    	break;
    	    default:
    	    	((z_double*)data)->data = (float)item->valuedouble;
    	    	((z_double*)data)->flag = 1;
    	    	break;
    	}
        return item->valuedouble;
    }
    switch(type)
	{
		case CSON_TYPE_FLOAT:
			((z_float*)data)->data = 0.0f;
			((z_float*)data)->flag = 0;
			break;
		case CSON_TYPE_DOUBLE:
			((z_double*)data)->data = 0.0f;
			((z_double*)data)->flag = 0;
			break;
		default:
			((z_double*)data)->data = 0.0f;
			((z_double*)data)->flag = 0;
			break;
	}
    return 0.0;
}


/**
 * @brief 解析JSON字符串数据
 * 
 * @param json JSON对象
 * @param key key
 * @return char* 解析出的字符串
 */
char* csonDecodeString(cJSON *json, char *key)
{
    char *p = NULL;
    char *str = NULL;
    short strLen = 0;
    cJSON *item = key ? cJSON_GetObjectItem(json, key) : json;
    if (item && item->type == cJSON_String)
    {
        str = item->valuestring;
        if (item->valuestring)
        {
            strLen = strlen(str);
            p = cson.malloc(strLen + 1);
            if (p)
            {
                memcpy((void *)p, (void *)str, strLen);
                *(p + strLen) = 0;
                return p;
            }
        }
    }
    return NULL;
}

/**
 * @brief 解析JSON字符串数据
 *
 * @param json JSON对象
 * @param key key
 * @return char* 解析出的字符串
 */
char* csonDecodeString2(cJSON *json, char *key, z_string* data)
{
    char *str = NULL;
    short strLen = 0;
    cJSON *item = key ? cJSON_GetObjectItem(json, key) : json;
    if (item && item->type == cJSON_String)
    {
        str = item->valuestring;
        if (item->valuestring)
        {
        	strLen = strlen(str);
			data->data = cson.malloc(strLen + 1);
			if (data->data)
			{
				memcpy((void *)data->data, (void *)str, strLen);
				*(data->data + strLen) = 0;
				data->flag = 1;
				return data->data;
			}
			data->flag = 0;
			return NULL;
        }
    }
    data->data = NULL;
    data->flag = 0;
    return NULL;
}


/**
 * @brief 解析JOSN布尔型数据
 * 
 * @param json JSON对象
 * @param key key
 * @return char 解析出的bool
 */
char csonDecodeBool(cJSON *json, char *key)
{
	printf("[%s]key:%s\n", __func__, key);
    cJSON *item = cJSON_GetObjectItem(json, key);
    if (item && item->type == cJSON_True)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief 解析JOSN布尔型数据
 *
 * @param json JSON对象
 * @param key key
 * @return char 解析出的bool
 */
char csonDecodeBool2(cJSON *json, char *key, z_bool* data)
{
	printf("[%s]key:%s\n", __func__, key);
    cJSON *item = cJSON_GetObjectItem(json, key);
    if (item && item->type == cJSON_True)
    {
    	data->data = 1;
    	data->flag = 1;
        return 1;
    }
    data->data = 0;
    data->flag = 0;
    return 0;
}


/**
 * @brief 解析CsonList数据
 * 
 * @param json JSON对象
 * @param key key
 * @param model CsonList成员数据模型
 * @param modelSize SconList成员模型数量
 * @return void* CsonList对象
 */
void *csonDecodeList(cJSON *json, char *key, CsonModel *model, int modelSize)
{
    CsonList *list = NULL;
    cJSON *array = cJSON_GetObjectItem(json, key);

    if (array && array->type == cJSON_Array)
    {
    	short i = 0;
        for (i = 0; i < cJSON_GetArraySize(array); i++)
        {
            void *obj = csonDecodeObject(cJSON_GetArrayItem(array, i), model, modelSize);
            if (csonIsBasicListModel(model))
            {
                list = csonListAdd(list, (void *)(*((size_t *)obj)));
                cson.free(obj);
            }
            else
            {
                list = csonListAdd(list, obj);
            }
        }
    }
    return list;
}


/**
 * @brief 解析数组
 * 
 * @param json json对象
 * @param key key
 * @param base 数组基址
 * @param elementType 数组元素类型
 * @param arraySize 数组大小
 */
void csonDecodeArray(cJSON *json, char *key, void * base, CsonType elementType, short arraySize)
{
    cJSON *array = cJSON_GetObjectItem(json, key);
    cJSON *item;
    char *str;
    short strLen;

    if (array && array->type == cJSON_Array)
    {
    	short i = 0;
        for (i = 0; i < cJSON_GetArraySize(array); i++)
        {
            item = cJSON_GetArrayItem(array, i);
            switch (elementType)
            {
            case CSON_TYPE_CHAR:
                *(char *)((size_t)base + (i * sizeof(char))) = (char)item->valueint;
                break;
            case CSON_TYPE_SHORT:
                *(short *)((size_t)base + (i * sizeof(short))) = (short)item->valueint;
                break;
            case CSON_TYPE_INT:
                *(int *)((size_t)base + (i * sizeof(int))) = (int)item->valueint;
                break;
            case CSON_TYPE_LONG:
                *(long *)((size_t)base + (i * sizeof(long))) = (long)item->valueint;
                break;
            case CSON_TYPE_FLOAT:
                *(float *)((size_t)base + (i * sizeof(float))) = (float)item->valuedouble;
                break;
            case CSON_TYPE_DOUBLE:
                *(double *)((size_t)base + (i * sizeof(double))) = (double)item->valuedouble;
                break;
            case CSON_TYPE_STRING:
                strLen = strlen(item->valuestring);
                str = cson.malloc(strLen + 1);
                memcpy(str, item->valuestring, strLen);
                *(str + strLen) = 0;
                *(size_t *)((size_t)base + (i * sizeof(size_t))) = (size_t)str;
                break;
            default:
                break;
            }
        }
    }
}


/**
 * @brief 解析JSON对象
 * 
 * @param json JSON对象
 * @param model 数据模型
 * @param modelSize 数据模型数量
 * @return void* 解析得到的对象
 */
void *csonDecodeObject(cJSON *json, CsonModel *model, int modelSize)
{
    CSON_ASSERT(json, return NULL);

    if (json->type == cJSON_NULL) {
        return NULL;
    }

    short objSize = 0;
    short i = 0;

    for (i = 0; i < modelSize; i++)
    {
        if (model[i].type == CSON_TYPE_OBJ)
        {
            objSize = model[i].param.objSize;
        }
    }
    void *obj = cson.malloc(objSize);
    CSON_ASSERT(obj, return NULL);

    for (i = 0; i < modelSize; i++)
    {
        switch (model[i].type)
        {
        case CSON_TYPE_CHAR:
        case CSON_TYPE_SHORT:
        case CSON_TYPE_INT:
        case CSON_TYPE_LONG:
        	csonDecodeNumber2(json, model[i].key, (void *)((size_t)obj + model[i].offset), model[i].type);
            break;
        case CSON_TYPE_FLOAT:
        case CSON_TYPE_DOUBLE:
           csonDecodeDouble2(json, model[i].key,  (void *)((size_t)obj + model[i].offset), model[i].type);
            break;
        case CSON_TYPE_BOOL:
        	csonDecodeBool2(json, model[i].key, (z_bool*)((size_t)obj + model[i].offset));
            break;
        case CSON_TYPE_STRING:
        	csonDecodeString2(json, model[i].key, (z_string *)(((size_t)obj + model[i].offset)));
            break;
        case CSON_TYPE_LIST:
            *(size_t *)((size_t)obj + model[i].offset) = (size_t)csonDecodeList(json,
            model[i].key, model[i].param.sub.model, model[i].param.sub.size);
            break;
        case CSON_TYPE_STRUCT:
            *(size_t *)((size_t)obj + model[i].offset) = (size_t)csonDecodeObject(
                cJSON_GetObjectItem(json, model[i].key), 
                model[i].param.sub.model, model[i].param.sub.size);
            break;
        case CSON_TYPE_ARRAY:
            csonDecodeArray(json, model[i].key, (void *)((size_t)obj + model[i].offset), 
                model[i].param.array.eleType, model[i].param.array.size);
            break;
        case CSON_TYPE_JSON:
            *(size_t *)((size_t)obj + model[i].offset) = (size_t)cJSON_PrintUnformatted(
                cJSON_GetObjectItem(json, model[i].key));
            break;
        default:
            break;
        }
    }
    return obj;
}


/**
 * @brief 解析JSON字符串
 * 
 * @param jsonStr json字符串
 * @param model 数据模型
 * @param modelSize 数据模型数量
 * @return void* 解析得到的对象
 */
void *csonDecode(const char *jsonStr, CsonModel *model, int modelSize)
{
    void *obj;
    cJSON *json = cJSON_Parse(jsonStr);
    CSON_ASSERT(json, return NULL);
    obj = csonDecodeObject(json, model, modelSize);
    cJSON_Delete(json);
    return obj;
}



/**
 * @brief 数字编码编码JSON
 * 
 * @param json json对象
 * @param key key
 * @param num 数值
 */
void csonEncodeNumber(cJSON *json, char *key, double num)
{
    if (key)
    {
        cJSON_AddNumberToObject(json, key, num);
    }
    else
    {
        json->type = cJSON_Number;
        json->valuedouble = num;
        json->valueint = (int)num;
    }
}


/**
 * @brief 字符串编码编码JSON
 * 
 * @param json json对象
 * @param key key
 * @param str 字符串
 */
static char* csonEncodeString_strdup(const char* str)
{
      size_t len;
      char* copy;

      len = strlen(str) + 1;
      if (!(copy = (char*)cson.malloc(len))) return 0;
      memcpy(copy,str,len);
      return copy;
}

void csonEncodeString(cJSON *json, char *key, char *str)
{
    if (key)
    {
        cJSON_AddStringToObject(json, key, str);
    }
    else
    {
        json->type = cJSON_String;
        json->valuestring = csonEncodeString_strdup(str);  
    }
}


/**
 * @brief CsonList编码成JSON对象
 * 
 * @param list CsonList对象
 * @param model 数据模型
 * @param modelSize 数据模型数量
 * @return cJSON* 编码得到的JOSN对象
 */
cJSON* csonEncodeList(CsonList *list, CsonModel *model, int modelSize)
{
    cJSON *root = cJSON_CreateArray();
    cJSON *item;
    CsonList *p = list;

    while (p)
    {
        if (p->obj)
        {
            if (csonIsBasicListModel(model))
            {
                item = csonEncodeObject(&(p->obj), model, modelSize);
            }
            else
            {
                item = csonEncodeObject(p->obj, model, modelSize);
            }
            cJSON_AddItemToArray(root, item);
        }
        p = p->next;
    }
    return root;
}


/**
 * @brief 数组编码成JSON对象
 * 
 * @param base 数组基址
 * @param elementType 数组元素类型
 * @param arraySize 数组大小
 * @return cJSON* 编码得到的JOSN对象
 */
cJSON* csonEncodeArray(void *base, CsonType elementType, short arraySize)
{
    cJSON *root = cJSON_CreateArray();
    cJSON *item;
    short i = 0;
    for (i = 0; i < arraySize; i++)
    {
        switch (elementType)
        {
        case CSON_TYPE_CHAR:
            item = cJSON_CreateNumber(*(char *)((size_t)base + (i * sizeof(char))));
            break;
        case CSON_TYPE_SHORT:
            item = cJSON_CreateNumber(*(short *)((size_t)base + (i * sizeof(short))));
            break;
        case CSON_TYPE_INT:
            item = cJSON_CreateNumber(*(int *)((size_t)base + (i * sizeof(int))));
            break;
        case CSON_TYPE_LONG:
            item = cJSON_CreateNumber(*(long *)((size_t)base + (i * sizeof(long))));
            break;
        case CSON_TYPE_FLOAT:
            item = cJSON_CreateNumber(*(float *)((size_t)base + (i * sizeof(float))));
            break;
        case CSON_TYPE_DOUBLE:
            item = cJSON_CreateNumber(*(double *)((size_t)base + (i * sizeof(double))));
            break;
        case CSON_TYPE_STRING:
            item = cJSON_CreateString((char *)*(size_t *)((size_t)base + (i * sizeof(size_t))));
            break;
        default:
            break;
        }
        cJSON_AddItemToArray(root, item);
    }
    return root;
}


/**
 * @brief 编码JSON对象
 * 
 * @param obj 对象
 * @param model 数据模型
 * @param modelSize 数据模型数量
 * @return cJSON* 编码得到的json对象
 */
cJSON* csonEncodeObject(void *obj, CsonModel *model, int modelSize)
{
    if (!obj) {
        return cJSON_CreateNull();
    }
    cJSON *root = cJSON_CreateObject();
    short i = 0;

    for (i = 0; i < modelSize; i++)
    {
        switch (model[i].type)
        {
        case CSON_TYPE_CHAR:
            csonEncodeNumber(root, model[i].key, *(char *)((size_t)obj + model[i].offset));
            break;
        case CSON_TYPE_SHORT:
            csonEncodeNumber(root, model[i].key, *(short *)((size_t)obj + model[i].offset));
            break;
        case CSON_TYPE_INT:
            csonEncodeNumber(root, model[i].key, *(int *)((size_t)obj + model[i].offset));
            break;
        case CSON_TYPE_LONG:
            csonEncodeNumber(root, model[i].key, *(long *)((size_t)obj + model[i].offset));
            break;
        case CSON_TYPE_FLOAT:
            csonEncodeNumber(root, model[i].key, *(float *)((size_t)obj + model[i].offset));
            break;
        case CSON_TYPE_DOUBLE:
            csonEncodeNumber(root, model[i].key, *(double *)((size_t)obj + model[i].offset));
            break;
        case CSON_TYPE_BOOL:
            cJSON_AddBoolToObject(root, model[i].key, *(char *)((size_t)obj + model[i].offset)); //如果是false， true类型的，则用这个
//            cJSON_AddBoolToObject(root, model[i].key, *(char *)((size_t)obj + model[i].offset)); //如果是0, 1类型的，则用这个
            break;
        case CSON_TYPE_STRING:
            if ((char *)(*(size_t *)((size_t)obj + model[i].offset)))
            {
                csonEncodeString(root, model[i].key, (char *)(*(size_t *)((size_t)obj + model[i].offset)));
            }
            break;
        case CSON_TYPE_LIST:
            if ((CsonList *)*(size_t *)((size_t)obj + model[i].offset))
            {
                cJSON_AddItemToObject(root, model[i].key, 
                    csonEncodeList((CsonList *)*(size_t *)((size_t)obj + model[i].offset),
                        model[i].param.sub.model, model[i].param.sub.size));
            }
            break;
        case CSON_TYPE_STRUCT:
            if ((void *)(*(size_t *)((size_t)obj + model[i].offset)))
            {
                cJSON_AddItemToObject(root, model[i].key, csonEncodeObject(
                    (void *)(*(size_t *)((size_t)obj + model[i].offset)),
                    model[i].param.sub.model, model[i].param.sub.size));
            }
            break;
        case CSON_TYPE_ARRAY:
            cJSON_AddItemToObject(root, model[i].key, csonEncodeArray(
                (void *)((size_t)obj + model[i].offset),
                model[i].param.array.eleType, model[i].param.array.size));
            break;
        case CSON_TYPE_JSON:
            if ((char *)(*(size_t *)((size_t)obj + model[i].offset)))
            {
                cJSON_AddItemToObject(root, model[i].key, 
                    cJSON_Parse((char *)(*(size_t *)((size_t)obj + model[i].offset))));
            }
            break;
        default:
            break;
        }
    }
    return root;
}


/**
 * @brief 编码成json字符串
 * 
 * @param obj 对象
 * @param model 数据模型
 * @param modelSize 数据模型数量
 * @param bufferSize 分配给json字符串的空间大小
 * @param fmt 是否格式化json字符串
 * @return char* 编码得到的json字符串
 */
char* csonEncode(void *obj, CsonModel *model, int modelSize)
{
    cJSON *json = csonEncodeObject(obj, model, modelSize);
    CSON_ASSERT(json, return NULL);
    char *jsonStr = cJSON_Print(json);
    cJSON_Delete(json);
    return jsonStr;
}


/**
 * @brief 编码成json字符串
 * 
 * @param obj 对象
 * @param model 数据模型
 * @param modelSize 数据模型数量
 * @return char* 编码得到的json字符串
 */
char* csonEncodeUnformatted(void *obj, CsonModel *model, int modelSize)
{
    cJSON *json = csonEncodeObject(obj, model, modelSize);
    CSON_ASSERT(json, return NULL);
    char *jsonStr = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    return jsonStr;
}


/**
 * @brief 释放CSON解析出的对象
 * 
 * @param obj 对象
 * @param model 对象模型
 * @param modelSize 对象模型数量
 */
void csonFree(void *obj, CsonModel *model, int modelSize)
{
    CsonList *list, *p;
    size_t *tmpNode;
    short i = 0;

    for (i = 0; i < modelSize; i++)
    {
        switch ((int)model[i].type)
        {
        case CSON_TYPE_CHAR:
        case CSON_TYPE_SHORT:
        case CSON_TYPE_INT:
        case CSON_TYPE_LONG:
        case CSON_TYPE_FLOAT:
        case CSON_TYPE_DOUBLE:
            break;
        case CSON_TYPE_STRING:
        case CSON_TYPE_JSON:
            cson.free(((z_string *)((size_t)obj + model[i].offset))->data);
            break;
        case CSON_TYPE_LIST:
            list = (CsonList *)*(size_t *)((size_t)obj + model[i].offset);
            while (list)
            {
                p = list;
                list = list->next;
                if (p->obj)
                {
                    if (csonIsBasicListModel(model[i].param.sub.model))
                    {
                        tmpNode = cson.malloc(sizeof(size_t));
                        *tmpNode = (size_t)(p->obj);
                    }
                    else
                    {
                        tmpNode = p->obj;
                    }
                    csonFree(tmpNode, 
                        model[i].param.sub.model, model[i].param.sub.size);
                }
                cson.free(p);
            }
            break;
        case CSON_TYPE_STRUCT:
            csonFree((void *)(*(size_t *)((size_t)obj + model[i].offset)), 
                model[i].param.sub.model, model[i].param.sub.size);
            break;
        case CSON_TYPE_ARRAY:
            if (model[i].param.array.eleType == CSON_TYPE_STRING)
            {
            	short j = 0;
                for (j = 0; j< model[i].param.array.size; j++)
                {
                    if (*(size_t *)((size_t)obj + model[i].offset + (j << 2)))
                    {
                        cson.free((void *)*(size_t *)((size_t)obj + model[i].offset + (j << 2)));
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    cson.free(obj);
}


/**
 * @brief 释放cson编码生成的json字符串
 * 
 * @param jsonStr json字符串
 */
void csonFreeJson(const char *jsonStr)
{
    CSON_ASSERT(jsonStr, return);
    cson.free((void *)jsonStr);
}


/**
 * @brief CSON链表添加节点
 * 
 * @param list 链表
 * @param obj 节点对象
 * @return CsonList 链表
 */
CsonList* csonListAdd(CsonList *list, void *obj)
{
    if (!list)
    {
        list = cson.malloc(sizeof(CsonList));
        if (!list)
        {
            return NULL;
        }
        list->next = NULL;
        list->obj = NULL;
    }
    CsonList *p = list;
    while (p->next)
    {
        p = p->next;
    }
    if (!p->obj)
    {
        p->obj = obj;
        p->next = NULL;
    }
    else
    {
        CsonList *node = cson.malloc(sizeof(CsonList));
        if (node)
        {
            node->obj = obj;
            node->next = NULL;
            p->next = node;
        }
    }
    return list;
}


/**
 * @brief CSON链表删除节点
 * 
 * @param list 链表
 * @param obj 节点对象
 * @param freeMem 释放内存
 * @return CsonList 链表
 */
CsonList *csonListDelete(CsonList *list, void *obj, char freeMem)
{
    CSON_ASSERT(list, return NULL);

    CsonList head = {0};
    head.next = list;
    CsonList *p = &head;
    CsonList *tmp;
    while (p->next)
    {
        if (p->next->obj && p->next->obj == obj)
        {
            tmp = p->next;
            p->next = p->next->next ? p->next->next : NULL;
            if (freeMem)
            {
                cson.free(tmp->obj);
                cson.free(tmp);
            }
            break;
        }
        p = p->next;
    }
    return head.next;
}


/**
 * @brief CSON新字符串
 * 
 * @param src 源字符串
 * @return char* 新字符串
 * @note 此函数用于复制字符串，建议对结构体中字符串成员赋值时，使用此函数，
 *       方便使用`csonFree`进行内存释放
 */
char* csonNewString(const char *src)
{
    int len = strlen(src);
    char *dest = cson.malloc(len + 1);
    strcpy(dest, src);
    return dest;
}
