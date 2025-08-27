#ifndef CUSTOMS_WAD_H
#define CUSTOMS_WAD_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define WAD_HEADER_IDENTIFICATION_SIZE 4
#define LUMP_ENTRY_MAX_NAME_SIZE 8

#pragma pack(push, 1)
typedef struct WadHeader {
    char Id[WAD_HEADER_IDENTIFICATION_SIZE];
    uint32_t NumLumps;
    uint32_t DirOffset;
} WadHeader_t;

typedef struct LumpDirEntry {
    uint32_t Offset;
    uint32_t Size;
    char Name[LUMP_ENTRY_MAX_NAME_SIZE];
} LumpDirEntry_t;
#pragma pack(pop)

typedef struct WadFile {
    FILE* FilePtr;
    WadHeader_t Header;
    LumpDirEntry_t* Directory; // dynamically allocated for reader
} WadFile_t;

void Wad_Create(WadFile_t* File, const char* FileName, char Id[WAD_HEADER_IDENTIFICATION_SIZE], uint32_t NumLumps);
void Wad_CreateReader(WadFile_t* File, const char* FileName);

void Lump_SetName(LumpDirEntry_t* Entry, const char* LumpName);
void Lump_Write(WadFile_t* File, void* Data, uint32_t Size, LumpDirEntry_t* Entry);

//OUTDATED
void Lump_Read(WadFile_t* File, LumpDirEntry_t* Entry, void* Buffer);

void Lump_WriteEntries(WadFile_t* File, LumpDirEntry_t* Entries);

void Wad_End(WadFile_t* File);
void Wad_EndReader(WadFile_t* File);

#endif
