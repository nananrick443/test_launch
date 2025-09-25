//	gameinit.c
/*
	さいたまゲーム(仮)
	初期化モジュール
	Coded by Kolorz.
	( 02/10/27 )
*/



/*  ■ Window生成前にコールされる初期化関数 ■  */
void main(void)
{
	SetWindow(1,640,480,8);	//  640*480_WindowMode
	SetDisplayMode(0,1);	//  0:WindowMode  1:FullScreenMode(8bpp)
}



/*_EOF_*/