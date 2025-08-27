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

//DOXYGEN BY CHAT_GPT

/**
 * @brief Creates a new WAD file for writing.
 * 
 * Initializes the WAD header and opens the file in write mode.
 * 
 * @param File Pointer to the WadFile_t structure to initialize.
 * @param FileName Name of the WAD file to create.
 * @param Id WAD type identifier (e.g., "IWAD" or "PWAD").
 * @param NumLumps Number of lumps that will be written.
 */
void Wad_Create(WadFile_t* File, const char* FileName, char Id[WAD_HEADER_IDENTIFICATION_SIZE], uint32_t NumLumps);

/**
 * @brief Opens an existing WAD file for reading.
 * 
 * Reads the WAD header and allocates memory for the lump directory.
 * 
 * @param File Pointer to the WadFile_t structure to initialize.
 * @param FileName Name of the WAD file to read.
 */
void Wad_CreateReader(WadFile_t* File, const char* FileName);

/**
 * @brief Sets the name of a lump directory entry.
 * 
 * @param Entry Pointer to the LumpDirEntry_t structure.
 * @param LumpName Name to assign to the lump (max 8 characters).
 */
void Lump_SetName(LumpDirEntry_t* Entry, const char* LumpName);

/**
 * @brief Writes data to a lump in the WAD file.
 * 
 * Updates the lump entry with the offset and size.
 * 
 * @param File Pointer to the WadFile_t structure.
 * @param Data Pointer to the data to write.
 * @param Size Size of the data in bytes.
 * @param Entry Pointer to the LumpDirEntry_t structure for this lump.
 */
void Lump_Write(WadFile_t* File, void* Data, uint32_t Size, LumpDirEntry_t* Entry);

/**
 * @brief Reads data from a lump in the WAD file.
 * 
 * Copies lump data into the provided buffer.
 * 
 * @param File Pointer to the WadFile_t structure.
 * @param Entry Pointer to the LumpDirEntry_t structure of the lump to read.
 * @param Buffer Pointer to a memory buffer to store the lump data.
 */
void Lump_Read(WadFile_t* File, LumpDirEntry_t* Entry, void* Buffer);

/**
 * @brief Writes the lump directory entries to the WAD file.
 * 
 * Updates the directory offset in the WAD header.
 * 
 * @param File Pointer to the WadFile_t structure.
 * @param Entries Pointer to an array of LumpDirEntry_t structures.
 */
void Lump_WriteEntries(WadFile_t* File, LumpDirEntry_t* Entries);

/**
 * @brief Finalizes a WAD file after writing.
 * 
 * Writes the updated header at the start and closes the file.
 * 
 * @param File Pointer to the WadFile_t structure.
 */
void Wad_End(WadFile_t* File);

/**
 * @brief Cleans up resources used for reading a WAD file.
 * 
 * Frees allocated memory and closes the file pointer.
 * 
 * @param File Pointer to the WadFile_t structure.
 */
void Wad_EndReader(WadFile_t* File);

#endif
