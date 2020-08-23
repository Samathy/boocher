.DEFAULT_GOAL := default

CC = g++
INC = libs/include
LIBS = libs/lib

OUTFILE = build/boocher

source = source/boocher.cpp

default:
	g++ -I $(INC) -L$(LIBS) -o $(OUTFILE) $(source) $(LIBS)/libmgclient.a -lssl -lcrypto
