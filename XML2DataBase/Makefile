include Makefile.define

# 默认超作
default:all

# 主应用文件列表
PATS = ./TinyXML/tinystr.o ./TinyXML/tinyxml.o ./TinyXML/tinyxmlerror.o ./TinyXML/tinyxmlparser.o \
	   XmlOpeation.o ClassCreate.o DBOperationCreate.o XML2DataBase.o PoolTest.o DBServerCreate.o \
	   ClassCreatePool.o
LIB_BASE_OBJS = tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o \
				XmlOpeation.o ClassCreate.o DBOperationCreate.o XML2DataBase.o PoolTest.o DBServerCreate.o \
				ClassCreatePool.o
LIB_BASE = XML2DataBase

all: XML2DataBase clear
			
# 主应用程序
all:$(LIB_BASE) Makefile

$(LIB_BASE):$(PATS)
	$(CC) -rdynamic -o $(LIB_BASE) $(LIB_BASE_OBJS) $(LIBS)

# 清理
clean:
	rm -rf *.o  $(LIB_BASE) $(LIB_BASE_OBJS)
clear:	
	mv XML2DataBase ./Bin
cl:	
	rm -rf *.o 
