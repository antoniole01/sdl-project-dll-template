#include <windows.h>
#include <iostream>

#include "test.h"
#include "pch.h"

#pragma warning(disable:4505)

typedef uint64_t uint64;
typedef int32_t  int32;
typedef int32    bool32;
typedef uint32_t uint32;

#define WIN32_STATE_FILE_NAME_COUNT MAX_PATH

struct win32_offscreen_buffer
{
   // NOTE(ME): Pixels are always 32-bits wide, Memory Order BB GG RR XX
   BITMAPINFO Info;
   void *Memory;
   int Width;
   int Height;
   int Pitch;
   int BytesPerPixel;
};

struct win32_game_code{
   HMODULE GameCodeDLL;
   FILETIME DLLLastWriteTime;
   game_update_and_render *UpdateAndRender;

   bool32 IsValid;
};

struct win32_state{
   uint64 TotalSize;
   //void *GameMemoryBlock;

   char EXEFileName[WIN32_STATE_FILE_NAME_COUNT];
   char *OnePastLastEXEFileNameSlash;
};

global_variable bool32 GlobalRunning;

global_variable win32_offscreen_buffer GlobalBackbuffer;

internal void
CatStrings(size_t SourceACount, char *SourceA, size_t SourceBCount, char *SourceB, size_t DestCount, char *Dest){
   for(int Index = 0;Index < (int)SourceACount;++Index){
      *Dest++ = *SourceA++;
   }
   for(int Index = 0;Index < (int)SourceBCount;++Index){
      *Dest++ = *SourceB++;
   }
   *Dest++ = 0;
}

internal void
Win32GetEXEFileName(win32_state *State){
   DWORD SizeOfFilename = GetModuleFileNameA(0, State->EXEFileName, sizeof(State->EXEFileName));
   State->OnePastLastEXEFileNameSlash = State->EXEFileName;
   for(char *Scan = State->EXEFileName;*Scan;++Scan){
      if(*Scan == '\\'){
			State->OnePastLastEXEFileNameSlash = Scan + 1;
      }
   }
}

internal int
StringLength(char *String){
   int Count = 0;
   while(*String++){
      ++Count;
   }
   return(Count);
}

internal void
Win32BuildEXEPathFileName(win32_state *State, char *FileName, int DestCount, char *Dest){
   CatStrings(State->OnePastLastEXEFileNameSlash - State->EXEFileName, State->EXEFileName,
				  StringLength(FileName), FileName,
				  DestCount, Dest);
}

inline FILETIME
Win32GetLastWriteTime(char *Filename){
   FILETIME LastWriteTime = {};

   WIN32_FILE_ATTRIBUTE_DATA Data;
   if(GetFileAttributesEx(Filename, GetFileExInfoStandard, &Data)){
      LastWriteTime = Data.ftLastWriteTime;
   }

   return(LastWriteTime);
}

internal win32_game_code
Win32LoadGameCode(char *SourceDLLName, char *TempDLLName){
   win32_game_code Result = {};

   Result.DLLLastWriteTime = Win32GetLastWriteTime(SourceDLLName);

   CopyFile(SourceDLLName, TempDLLName, FALSE);
   Result.GameCodeDLL = LoadLibraryA(TempDLLName);
   if(Result.GameCodeDLL){
      Result.UpdateAndRender = (game_update_and_render *)
			GetProcAddress(Result.GameCodeDLL, "GameUpdateAndRender");

      Result.IsValid = (bool32)(Result.UpdateAndRender);
   }

   if(!Result.IsValid){
      Result.UpdateAndRender = 0;
   }

   return(Result);
}

internal void
Win32UnloadGameCode(win32_game_code *GameCode){
   if(GameCode->GameCodeDLL){
      FreeLibrary(GameCode->GameCodeDLL);
      GameCode->GameCodeDLL = 0;
   }

   GameCode->IsValid = false;
   GameCode->UpdateAndRender = 0;
}

int main()
{
	
   win32_state Win32State = {};
   Win32GetEXEFileName(&Win32State);

   char SourceGameCodeDLLFullPath[WIN32_STATE_FILE_NAME_COUNT];
   Win32BuildEXEPathFileName(&Win32State,"test.dll", sizeof(SourceGameCodeDLLFullPath),SourceGameCodeDLLFullPath);
   char TempGameCodeDLLFullPath[WIN32_STATE_FILE_NAME_COUNT];
   Win32BuildEXEPathFileName(&Win32State,"test_temp.dll", sizeof(TempGameCodeDLLFullPath),TempGameCodeDLLFullPath);
	
   GlobalRunning = true;

   win32_game_code Game = Win32LoadGameCode(SourceGameCodeDLLFullPath, TempGameCodeDLLFullPath);
   uint32 LoadCounter = 0;
	
   while(GlobalRunning)
   {
		
      FILETIME NewDLLWriteTime = Win32GetLastWriteTime(SourceGameCodeDLLFullPath);
      if(CompareFileTime(&NewDLLWriteTime, &Game.DLLLastWriteTime) != 0)
      {
			Win32UnloadGameCode(&Game);
			Game = Win32LoadGameCode(SourceGameCodeDLLFullPath,TempGameCodeDLLFullPath);

			LoadCounter = 0;
      }

		//
		//NOTE:me disable this to not receive quit input
		
      // if(GetAsyncKeyState(VK_ESCAPE))
		// {
		// 	GlobalRunning = false;
      // }

      game_offscreen_buffer Buffer = {};
      Buffer.Memory = GlobalBackbuffer.Memory;
      Buffer.Width = GlobalBackbuffer.Width;
      Buffer.Height = GlobalBackbuffer.Height;
      Buffer.BytesPerPixel = GlobalBackbuffer.BytesPerPixel;

      if(Game.UpdateAndRender)
		{
			Game.UpdateAndRender(&Buffer);
      }
   }
	
	destroy_Window();
	
   return(0);
}
