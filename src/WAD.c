#include "WAD.h"

void Wad_Create(WadFile_t* File, const char* FileName, char Id[WAD_HEADER_IDENTIFICATION_SIZE], uint32_t NumLumps) {
    File->FilePtr = fopen(FileName, "wb");
    WadHeader_t Header;
    memcpy(Header.Id, Id, 4);
    Header.NumLumps = NumLumps;
    Header.DirOffset = 0; //change later
    File->Header = Header;
    fwrite(&File->Header, sizeof(File->Header), 1, File->FilePtr);
};

void Wad_CreateReader(WadFile_t* File, const char* FileName) {
    File->FilePtr = fopen(FileName, "rb");
    if (!File->FilePtr) {
        perror("Failed to open WAD for reading");
        return;
    }
    fread(&File->Header, sizeof(WadHeader_t), 1, File->FilePtr);
    File->Directory = malloc(sizeof(LumpDirEntry_t) * File->Header.NumLumps);
    if (!File->Directory) {
        perror("Failed to allocate lump directory");
        fclose(File->FilePtr);
        return;
    }
    fseek(File->FilePtr, File->Header.DirOffset, SEEK_SET);
    fread(File->Directory, sizeof(LumpDirEntry_t), File->Header.NumLumps, File->FilePtr);
};

void Lump_SetName(LumpDirEntry_t* Entry, const char* LumpName) {
    memset(Entry->Name, 0, LUMP_ENTRY_MAX_NAME_SIZE);
    size_t len = strlen(LumpName);
    if(len > LUMP_ENTRY_MAX_NAME_SIZE) len = LUMP_ENTRY_MAX_NAME_SIZE;
    memcpy(Entry->Name, LumpName, len);
};

void Lump_Write(WadFile_t* File, void* Data, uint32_t Size, LumpDirEntry_t* Entry) {
    Entry->Offset = ftell(File->FilePtr);
    Entry->Size = Size;
    fwrite(Data, Size, 1, File->FilePtr);
};

void Lump_Read(WadFile_t* File, LumpDirEntry_t* Entry, void* Buffer) {
    fseek(File->FilePtr, Entry->Offset, SEEK_SET);
    fread(Buffer, Entry->Size, 1, File->FilePtr);
};

void Lump_WriteEntries(WadFile_t* File, LumpDirEntry_t* Entries) {
    File->Header.DirOffset = ftell(File->FilePtr);
    fwrite(Entries, sizeof(LumpDirEntry_t), File->Header.NumLumps, File->FilePtr);
};

void Wad_End(WadFile_t* File) {
    //File->Header.DirOffset = ftell(File->FilePtr);
    fseek(File->FilePtr, 0, SEEK_SET);
    fwrite(&File->Header, sizeof(File->Header), 1, File->FilePtr);
    fclose(File->FilePtr);
};

void Wad_EndReader(WadFile_t* File) {
    free(File->Directory);
    fclose(File->FilePtr);
};
