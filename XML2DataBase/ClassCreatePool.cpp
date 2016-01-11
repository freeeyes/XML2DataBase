#include "ClassCreatePool.h"

bool Create_Class_Pool_H(_XML_Proc& obj_XML_Proc)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

	//生成对应的pool头文件
	for(int i =0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		//判断是否有Pool需要声明
		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s_Pool.h", 
				obj_XML_Proc.m_sz_ProcName,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);

			FILE* pFile = fopen(szPathFile, "wb");
			if(NULL == pFile)
			{
				return false;
			}

			//将类名转换成大写
			char szHText[MAX_BUFF_50] = {'\0'};
			To_Upper_String(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name, szHText);

			sprintf_safe(szTemp, 200, "#ifndef _%s_POOL_H\n", szHText);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "#define _%s_POOL_H\n\n", szHText);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//添加引用基类头文件
			sprintf_safe(szTemp, 200, "#include \"%s.h\"\n\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//首先得到key的类型
			char szKeyType[MAX_BUFF_50] = {'\0'};
			for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
			{
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
				{
					sprintf_safe(szKeyType, MAX_BUFF_50, "%s", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
					break;
				}
			}

			//如果得不到Key类型，则不生成Pool代码
			if(strlen(szKeyType) == 0)
			{
				continue;
			}

			//声明map和vector
			sprintf_safe(szTemp, 200, "typedef vector<%s*> vec_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "typedef map<%s, %s*> map_%s;\n", 
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "typedef vector<%s> vec_Key_%s_List;\n\n", 
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//生成pool相应代码
			sprintf_safe(szTemp, 200, "//Pool Manager %s.\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "class %s_Pool\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "private:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t%s_Pool();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "public:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t~%s_Pool();\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//生成对应调用方法
			sprintf_safe(szTemp, 200, "\tvoid init();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//加载数据函数
			//sprintf_safe(szTemp, 200, "\tbool load_data();\n");
			//fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//保存数据函数
			//sprintf_safe(szTemp, 200, "\tbool save_data();\n");
			//fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tbool get_is_create();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tvoid set_is_create(bool bl_is_create);\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tsize_t get_st_poolsize();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\t%s* get_free_object(%s obj_%s);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\t%s* get_used_object(%s obj_%s);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tbool del_used_object(%s obj_%s);\n", 
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tint get_used_count();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tint get_pool_count();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//得到已使用的对象Key列表
			sprintf_safe(szTemp, 200, "\tvoid get_used_key_list(vec_Key_%s_List& obj_vec_Key_%s_List);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//重置所有对象
			sprintf_safe(szTemp, 200, "\tvoid clear();\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "private:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
			{
				sprintf_safe(szTemp, 200, "\tshm_id m_obj_shm_id;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			sprintf_safe(szTemp, 200, "\t%s* m_list_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tint m_list_Count;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tbool m_bl_Is_Create;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tsize_t m_st_PoolSize;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tvec_%s m_vec_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmap_%s m_map_%s;\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//添加单件代码
			sprintf_safe(szTemp, 200, "private:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tstatic %s_Pool* obj_%s_Pool_Instance;\n\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tclass CClear_%s_Instance\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tpublic:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tCClear_%s_Instance() {};\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t~CClear_%s_Instance()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tdelete obj_%s_Pool_Instance;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t};\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "public:\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tstatic %s_Pool* getInstance()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tif(obj_%s_Pool_Instance == NULL)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tstatic CClear_%s_Instance obj_%s_clr;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tobj_%s_Pool_Instance = new %s_Pool();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t\tobj_%s_Pool_Instance->init();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn obj_%s_Pool_Instance;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\tmap_%s& get_used_map()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn m_map_%s;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "};\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "#endif\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			fclose(pFile);
		}
	}

	return true;
}

bool Create_Class_Pool_CPP(_XML_Proc& obj_XML_Proc)
{
	char szTemp[1024]     = {'\0'};
	char szPathFile[200]  = {'\0'};

		//生成对应Pool代码
	for(int i = 0; i < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); i++)
	{
		//判断是否有Pool需要声明
		if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0 && strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key) > 0)
		{
			sprintf_safe(szPathFile, 200, "%s/DataWrapper/%s_Pool.cpp", 
				obj_XML_Proc.m_sz_ProcName,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);

			FILE* pFile = fopen(szPathFile, "wb");
			if(NULL == pFile)
			{
				return false;
			}

			sprintf_safe(szTemp, 200, "#include \"%s_Pool.h\"\n\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//sprintf_safe(szTemp, sizeof(szTemp), "#include \"DB_Op.h\"\n\n");
			//fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			if(obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool > 0)
			{
				//添加单件初始化
				sprintf_safe(szTemp, 200, "%s_Pool *%s_Pool::obj_%s_Pool_Instance = NULL;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			//首先得到key的类型
			char szKeyType[MAX_BUFF_50] = {'\0'};
			for(int j = 0; j < (int)obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info.size(); j++)
			{
				if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key, obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Column_Name) == 0)
				{
					sprintf_safe(szKeyType, MAX_BUFF_50, "%s", obj_XML_Proc.m_obj_vec_Table_Info[i].m_obj_vec_Column_Info[j].m_sz_Class_Type);
					break;
				}
			}

			//如果得不到Key类型，则不生成Pool代码
			if(strlen(szKeyType) == 0)
			{
				continue;
			}

			//填充函数
			//构造
			sprintf_safe(szTemp, 200, "%s_Pool::%s_Pool()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_list_%s = NULL;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_list_Count = 0;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_bl_Is_Create = true;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_st_PoolSize = 0;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//析构
			sprintf_safe(szTemp, 200, "%s_Pool::~%s_Pool()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) == 0)
			{
				sprintf_safe(szTemp, 200, "\tif(NULL != m_list_%s)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tdelete[] m_list_%s;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tm_list_%s = NULL;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				//释放共享内存连接
				sprintf_safe(szTemp, 200, "\tif(NULL != m_list_%s && m_obj_shm_id > 0)\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tClose_Share_Memory_API((char* )m_list_%s, m_obj_shm_id, get_st_poolsize());\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//初始化
			sprintf_safe(szTemp, 200, "void %s_Pool::init()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//这里判断是否有共享内存标签，如果有填充共享内存代码
			if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
			{
				sprintf_safe(szTemp, 200, "\tm_st_PoolSize = sizeof(%s) * %d;\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tm_list_%s = (%s* )Open_Share_Memory_API(%s, m_st_PoolSize, m_obj_shm_id, m_bl_Is_Create);\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tm_list_%s = new %s[%d];\n",
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}

			sprintf_safe(szTemp, 200, "\tif(NULL == m_list_%s)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_list_Count = %d;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			if (obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_IsDependFunc == 1)
			{
				//test
				sprintf_safe(szTemp, 200, "\tif(m_bl_Is_Create)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tload_data_from_%s(m_list_%s, m_list_Count);\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//test

				if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
				{
					//开始根据共享内存还原数据结构map和vector
					sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tif(m_bl_Is_Create == false)\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tif(true == m_list_%s[i].check_init())\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					//还原内存场景
					sprintf_safe(szTemp, 200, "\t\t\t\tm_map_%s.insert(map_%s::value_type(m_list_%s[i].get_%s(), &m_list_%s[i]));\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
			}
			else
			{
				//test
				sprintf_safe(szTemp, 200, "\tif(m_bl_Is_Create)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\tmap_%s mapObject = %s_Pool::get_used_map();\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass,
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			    sprintf_safe(szTemp, 200, "\t\tint iLoop = 0;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\tfor(map_%s::iterator b = mapObject.begin(); b != mapObject.end(); b++,iLoop++)\n", 
					obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\t\tif(iLoop < m_list_Count)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				sprintf_safe(szTemp, 200, "\t\t\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

				for(int iloop = 0; iloop < (int)obj_XML_Proc.m_obj_vec_Table_Info.size(); iloop++)
				{
					if(strcmp(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_DependClass, obj_XML_Proc.m_obj_vec_Table_Info[iloop].m_sz_Class_Name) == 0)
					{
						sprintf_safe(szTemp, 200, "\t\t\t\tm_list_%s[iLoop].set_%s(((%s*)b->second)->get_%s());\n",
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
							obj_XML_Proc.m_obj_vec_Table_Info[iloop].m_sz_Class_Name,
							obj_XML_Proc.m_obj_vec_Table_Info[iloop].m_sz_key);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						break;
					}
				}

				sprintf_safe(szTemp, 200, "\t\t\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				
				sprintf_safe(szTemp, 200, "\t\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//test

				if(strlen(obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_ShmKey) > 0)
				{
					//开始根据共享内存还原数据结构map和vector
					sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tif(m_bl_Is_Create == false)\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tif(true == m_list_%s[i].check_init())\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					//还原内存场景
					sprintf_safe(szTemp, 200, "\t\t\t\tm_map_%s.insert(map_%s::value_type(m_list_%s[i].get_%s(), &m_list_%s[i]));\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\telse\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t{\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
						obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					sprintf_safe(szTemp, 200, "\t}\n");
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				
			}

			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			/*
			sprintf_safe(szTemp, 200, "bool %s_Pool::load_data()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			
			sprintf_safe(szTemp, 200, "\treturn load_data_from_%s(m_list_%s, m_list_Count);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "bool %s_Pool::save_data()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\treturn save_data_to_%s(m_list_%s, m_list_Count);\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Table_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			*/


			sprintf_safe(szTemp, 200, "bool %s_Pool::get_is_create()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn m_bl_Is_Create;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "void %s_Pool::set_is_create(bool bl_is_create)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_bl_Is_Create = bl_is_create;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "size_t %s_Pool::get_st_poolsize()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn m_st_PoolSize;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//得到空余的对象
			sprintf_safe(szTemp, 200, "%s* %s_Pool::get_free_object(%s obj_%s)\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tif(m_vec_%s.size() <= 0)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn NULL;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t%s* p%s = NULL;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tvec_%s::iterator b = m_vec_%s.begin();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tp%s = (%s* )(*b);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmap_%s::iterator f = m_map_%s.find(obj_%s);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tif(f != m_map_%s.end())\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn (%s* )f->second;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_map_%s.insert(map_%s::value_type(obj_%s, p%s));\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_vec_%s.erase(b);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn p%s;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//得到正在使用的对象函数
			sprintf_safe(szTemp, 200, "%s* %s_Pool::get_used_object(%s obj_%s)\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmap_%s::iterator f = m_map_%s.find(obj_%s);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tif(f != m_map_%s.end())\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn (%s* )f->second;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\telse\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn NULL;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//归还正在使用的对象函数
			sprintf_safe(szTemp, 200, "bool %s_Pool::del_used_object(%s obj_%s)\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				szKeyType,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmap_%s::iterator f = m_map_%s.find(obj_%s);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tif(f != m_map_%s.end())\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t%s* p%s = (%s* )f->second;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tp%s->init();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tm_map_%s.erase(f);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tm_vec_%s.push_back(p%s);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn true;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\telse\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\treturn false;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//得到正在使用的对象个数函数
			sprintf_safe(szTemp, 200, "int %s_Pool::get_used_count()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn (int)m_map_%s.size();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//得到池里面总共的对象个数函数
			sprintf_safe(szTemp, 200, "int %s_Pool::get_pool_count()\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn (int)m_vec_%s.size();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			//得到已有的key对象的列表
			sprintf_safe(szTemp, 200, "void %s_Pool::get_used_key_list(vec_Key_%s_List& obj_vec_Key_%s_List)\n", 
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tfor(map_%s::iterator b = m_map_%s.begin();b != m_map_%s.end();b++)\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\t%s* pData = (%s* )b->second;\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tobj_vec_Key_%s_List.push_back(pData->m_obj_%s);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_key);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);


			//全部重置对象函数
			sprintf_safe(szTemp, 200, "void %s_Pool::clear()\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_vec_%s.clear();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tm_map_%s.clear();\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tmemset(m_list_%s, 0, m_st_PoolSize);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\tfor(int i = 0; i < %d; i++)\n", obj_XML_Proc.m_obj_vec_Table_Info[i].m_n_Class_Pool);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t\tm_vec_%s.push_back(&m_list_%s[i]);\n",
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name,
				obj_XML_Proc.m_obj_vec_Table_Info[i].m_sz_Class_Name);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t}\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

			sprintf_safe(szTemp, 200, "\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);	

			fclose(pFile);
		}
	}

	return true;
}

void Create_Pool_Proc(_XML_Proc& obj_XML_Proc)
{
	Create_Class_Pool_H(obj_XML_Proc);

	Create_Class_Pool_CPP(obj_XML_Proc);
}
