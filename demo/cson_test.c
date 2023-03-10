/**
 * @file cson_test.c
 * @author Letter (NevermindZZT@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-08-27
 *
 * @copyright (c) 2019 Letter
 *
 */

/*modified by zhouxin@20230310*/

#include "cson.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef DMALLOC_DEBUG
#include "dmalloc.h"
#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/
#if 1
struct subtest
{
    int id;
    char *test;
};

struct test
{
    char id;
    short num;
    int max;
    double value;
    char *name;
    struct subtest *sub;
    CsonList *list;
    char *str[2];
    CsonList *charList;
    CsonList *strList;
    char *subjson;
};

CsonModel subModel[] =
{
    CSON_MODEL_OBJ(struct subtest),
    CSON_MODEL_INT(struct subtest, id),
    CSON_MODEL_STRING(struct subtest, test)
};


CsonModel model[] =
{
    CSON_MODEL_OBJ(struct test),
    CSON_MODEL_CHAR(struct test, id),
    CSON_MODEL_SHORT(struct test, num),
    CSON_MODEL_INT(struct test, max),
    CSON_MODEL_DOUBLE(struct test, value),
    CSON_MODEL_STRING(struct test, name),
    CSON_MODEL_STRUCT(struct test, sub, subModel, sizeof(subModel)/sizeof(CsonModel)),
    CSON_MODEL_LIST(struct test, list, subModel, sizeof(subModel)/sizeof(CsonModel)),
    CSON_MODEL_ARRAY(struct test, str, CSON_TYPE_STRING, 2),
    CSON_MODEL_LIST(struct test, charList, CSON_MODEL_INT_LIST, CSON_BASIC_LIST_MODEL_SIZE),
    CSON_MODEL_LIST(struct test, strList, CSON_MODEL_STRING_LIST, CSON_BASIC_LIST_MODEL_SIZE),
    CSON_MODEL_JSON(struct test, subjson)
};

void csonTest(void)
{
    char *jsonStr = "{\"id\": 1, \"num\": 300, \"max\": 1000, \"value\": 10.3, \"name\": \"letter\", "
    "\"sub\": {\"id\": 20, \"test\": \"hello world\"},"
    " \"list\": [{\"id\": 21, \"test\": \"hello cson\"}, {\"id\": 22, \"test\": \"hello letter\"}],"
    "\"str\": [\"array1\", \"array2\"],"
    "\"charList\": [1, 12, 52], "
    "\"strList\": [\"str1\", \"str2\"],"
    "\"subjson\":{\"test\": \"hello\"}}";

    struct test *st = csonDecode(jsonStr, model, sizeof(model)/sizeof(CsonModel));
    printf("json 0x%08lx, id: %d, num: %d, max: %d, value: %f, name: %s\r\nsub: id: %d, test: %s\n",
        (unsigned long)st, st->id, st->num, st->max, st->value, st->name, st->sub ? st->sub->id : 0, st->sub ? st->sub->test : "null");
    printf("str: %s %s\n", st->str[0], st->str[1]);
    CsonList *p = st->list;
    while (p)
    {
        struct subtest *sst = p->obj;
        if (p->obj)
        {
            printf("list: id: %d, test: %s\n", sst->id, sst->test);
        }
        else
        {
            printf("list: null\n");
        }
        p = p->next;
    }
    p = st->charList;
    while (p)
    {
        long sst = (long)(p->obj);
        if (p->obj)
        {
            printf("list: long: %ld\n", sst);
        }
        p = p->next;
    }
    p = st->strList;
    while (p)
    {
        char *sst = p->obj;
        if (p->obj)
        {
            printf("list: str: %s\n", sst);
        }
        p = p->next;
    }
    printf("subjson: %s\n", st->subjson);

    char *root = csonEncode(st, model, sizeof(model)/sizeof(CsonModel));
    printf("encode: %s\n", root);

    csonFreeJson(root);
    csonFree(st, model, sizeof(model)/sizeof(CsonModel));
}
#endif
/************************************************************************/
/*                                                                      */
/************************************************************************/

/** 项目结构体 */
typedef struct
{
	z_int id;
	z_string name;
}project;

/** 仓库结构体 */
typedef struct
{
	z_bool id;
    z_string user;
    project *cson;
}hub;

/** 项目结构体数据模型 */
CsonModel projectModel[] =
{
    CSON_MODEL_OBJ(project),
    CSON_MODEL_INT2(project, id, "id1"),
    CSON_MODEL_STRING2(project, name, "name1"),
};

/** 仓库结构体数据模型 */
CsonModel hubModel[] =
{
    CSON_MODEL_OBJ(hub),
    CSON_MODEL_BOOL(hub, id),
    CSON_MODEL_STRING(hub, user),
    CSON_MODEL_STRUCT(hub, cson, projectModel, sizeof(projectModel)/sizeof(CsonModel))
};


void csonDemo(void)
{
    char *jsonDemo = "{\"id\": false, \"user\": \"Letter\", \"cson\": {\"id1\": 2, \"name1\": \"cson\"}}";
	//char *jsonDemo = "{\"id\": false, \"user\": \"Letter\", \"cson\": {\"name\": \"cson\"}}";
	//char *jsonDemo = "{\"id\": false, \"cson\": {\"id\": 2, \"name\": \"cson\"}}";
    /** 解析json */
    hub *pHub = csonDecode(jsonDemo, hubModel, sizeof(hubModel)/sizeof(CsonModel));
    printf("hub: id: %d, user: %s, project id: %s, project name: %s\r\n",
        pHub->id.data, pHub->user.flag == 1 ? pHub->user.data : "not exist", pHub->cson->id.flag == 1 ? "exist" : "not exist", pHub->cson->name.data);

    /** 序列化对象 */
    char *formatJson = csonEncode(pHub, hubModel, sizeof(hubModel)/sizeof(CsonModel));
    printf("format json: %s\r\n", formatJson);

    /** 释放结构体对象 */
    csonFree(pHub, hubModel, sizeof(hubModel)/sizeof(CsonModel));

    /** 释放序列化生成的json字符串 */
    csonFreeJson(formatJson);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

int main(int argc, char* argv[])
{

#ifdef DMALLOC_DEBUG
	dmalloc_debug_setup("debug=0x4e48503,inter=100,log=/tmp/dmalloc.log");
#endif
    csonInit(malloc, free);
    csonDemo();
    printf("\n");
    //csonTest();
    return 0;
}
