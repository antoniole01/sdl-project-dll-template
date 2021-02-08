#ifndef TEST_H
#define TEST_H

#define internal static
#define global_variable static

struct game_offscreen_buffer
{
   void *Memory;
   int Width;
   int Height;

   int BytesPerPixel;
};

struct game_state
{
   int GreenOffset;
   int BlueOffset;
	
};

#define GAME_UPDATE_AND_RENDER(name) void name(game_offscreen_buffer *Buffer)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

#endif // TEST_H
