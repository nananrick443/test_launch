//	gamestart.c
/*
	さいたまゲーム(仮)
	コアモジュール
	Coded by Kolorz.
	( 02/10/27 )
*/


/*  ■ Config的物体 ■  */
int UseKatakana = 0;  //  カタカナも使用（0:非使用  1:使用）



/*  ■ グローバル変数 ■  */
int gScreenMode;

//  文字の並び順
int TextArray_0[ 4] = {  //  １文字目
	0,1,2,3
};
int TextArray_1[12] = {  //  ２文字目
	1,2,3,
	0,2,3,
	0,1,3,
	0,1,2
};
int TextArray_2[24] = {  //  ３文字目＆４文字目
	2,3,  1,3,  1,2,
	2,3,  0,3,  0,2,
	1,3,  0,3,  0,1,
	1,2,  0,2,  0,1
};



/*  ■ このゲーム全体の初期化 ■  */
void Init(void)
{
	SetSystemMemoryUse(0);		//  ビデオメモリでヨロシク
	SetFillColor(0xffffffff);	//  塗り潰し色は白で
	
	gScreenMode = 0;
	
	//  共通パレットの設定
	LoadBitmap("./system/pallete.bmp",0,1);
	ReleaseBitmap(0);
}



/*  ■ 画像・音声の読み込み ■  */
void LoadObject(void)
{
	/*
		[ PlaneObject(s). ]
		10 : 文字(text.bmp)
		11 : ボタン説明(label.bmp)
		20 : タイトルロゴ(title.bmp)
		21 : 太陽(sun.bmp)
		22 : さいたま達(stmchar.bmp)
		23 : ×マーク(boo.bmp)
		
		[ SoundObject(s). ]
		10 : 「さいたま！」(stm0.wav)
		11 : 「さいたま～」(stm1.wav)
		12 : 「ぼ～ん」(boo.wav)
		13 : 「ready?」(ready.wav)
		14 : 「さいだま゛ぁ゛ぁ゛」(go.wav)
	*/
	
	LoadBitmap("./graphic/text.bmp",10,0);
	LoadBitmap("./graphic/label.bmp",11,0);
	LoadBitmap("./graphic/title.bmp",20,0);
	LoadBitmap("./graphic/sun.bmp",21,0);
	LoadBitmap("./graphic/stmchar.bmp",22,0);
	LoadBitmap("./graphic/boo.bmp",23,0);
	SetColorKeyPos(20,0,0);
	SetColorKeyPos(23,0,0);
	
	LoadWave("./sound/stm0.wav",10);
	LoadWave("./sound/stm1.wav",11);
	LoadWave("./sound/boo.wav",12);
	LoadWave("./sound/ready.wav",13);
	LoadWave("./sound/go.wav",14);
}



/*  ■ 画像・音声の解放 ■  */
void ReleaseObject(void)
{
	ReleaseBitmap(10);
	ReleaseBitmap(11);
	ReleaseBitmap(20);
	ReleaseBitmap(21);
	ReleaseBitmap(22);
	ReleaseBitmap(23);
	
	ReleaseWaveAll();
}



/*  ■ フルスクリーン←→Windowモードの切り替え ■  */
void SwitchScreenMode(void)
{
	gScreenMode = !(gScreenMode);
	ChangeDisplayMode( gScreenMode+1 );
}



/*  ■  シーン切り替え時の演出(？)効果  ■  */
void ToNextScene(int fncNextScene)
{
	//  激しく
	//  　未実装
	
	fncNextScene();  //  次のシーンをコール
}



/*  ■ タイトル画面 ■  */
int sceneTitle(void)
{
	//  変数定義
	int ResValue;
	int frameCnt;
	
	
	//  シーン初期化
	SetFPS(40);
	SetFillColor(0xffffffff);
	ClearSecondary();
	ResValue = 0;
	
	
	frameCnt = 0;
	loop
	{
		//  【 フレーム初期化 】
		ClearSecondary();
		KeyInput();
		
		
		//  【 描画 】
		BltFastRect(21,270,100,0,(frameCnt / 3 % 6)*100,100,100);
		BltFastRect(22,100,230+Rand(8),0,0,280,80);
		BltFastRect(22,320,200+Rand(8),0,80,280,110);
		Blt(20,120,(frameCnt%576)-48);
		
		
		//  【 音声 】
		if(!(frameCnt % 15)){
			PlayWave(10);
		}
		
		
		//  【 キー入力時挙動 】
		if( IsPushReturnKey() ){
			ResValue = 0;
			break;  //  [Enter]キーでゲーム開始
		}
		if( IsPushEscKey() ){
			ResValue = -1;
			break;  //  [ESC]キーで終了
		}
		if( IsPushKey(0x3E) ){
			SwitchScreenMode();  //  [F4]キーでスクリーンモード切り替え
		}
		
		
		//  【 Flipせよ 】
		halt;
		
		frameCnt++;
	}
	
	
	return( ResValue );
}



/*  ■ ゲーム画面 ■  */
int sceneGameMain(void)
{
	//  変数定義
	int ResValue;
	int frameCnt,timeCnt;
	
	int iBtn,iText;
	
	int StageNo;
	int TextNo , TextOrder[4] , TextType[4];
	int RndTblIndex[4];
	int BooCnt;
	int BtnValue , BtnValueArray[4] , BtnCode[4];
	int TextBltPosX[4],TextSrcPosX[4],TextSrcPosY[2];
	int BtnPushNum;
	
	
	//  シーン初期化
	SetFPS(40);
	SetFillColor(0xffffffff);
	ClearSecondary();
	ResValue = 0;
	
	BtnValueArray[0] = 1;  //  0001
	BtnValueArray[1] = 2;  //  0010
	BtnValueArray[2] = 4;  //  0100
	BtnValueArray[3] = 8;  //  1000
	
	BtnCode[0] = 0x2C;  //  [Z]キー
	BtnCode[1] = 0x2D;  //  [X]キー
	BtnCode[2] = 0x2E;  //  [C]キー
	BtnCode[3] = 0x2F;  //  [V]キー
	
	TextBltPosX[0] =  80;
	TextBltPosX[1] = 200;
	TextBltPosX[2] = 320;
	TextBltPosX[3] = 440;
	
	TextSrcPosX[0] =   0;
	TextSrcPosX[1] = 120;
	TextSrcPosX[2] = 240;
	TextSrcPosX[3] = 360;
	
	TextSrcPosY[0] =   0;
	TextSrcPosY[1] = 120;
	
	
	StageNo = 0;
	TextNo = 4;  //  問題を作る必要あり
	BooCnt = 0;
	BtnPushNum = 0;
	
	
	//  ＜01＞  「Ready?」状態
	PlayWave(13);
	
	for( frameCnt=0;  frameCnt<30;  frameCnt++ )
	{
		//  【 フレーム初期化 】
		ClearSecondary();
		KeyInput();
		
		
		//  【 描画 】
		BltFastRect( 21,270,(frameCnt*3)-10,0,0,100,100 );  //  太陽
		for( iText=0;  iText<4;  iText++ )
		{
			BltFastRect( 10,TextBltPosX[iText],180,TextSrcPosX[Rand(4)],0,120,120 );  //  文字
		}
		
		
		//  【 キー入力時挙動 】
		if( IsPushEscKey() ){
			return(-1);
			break;  //  [ESC]キーで終了
		}
		
		
		//  【 Flipせよ 】
		halt;
	}
	
	
	//  ＜02＞  「Ready?」の後の待ち
	for( frameCnt=0;  frameCnt<6;  frameCnt++ )
	{
		//  【 フレーム初期化 】
		ClearSecondary();
		KeyInput();
		
		
		//  【 描画 】
		BltFastRect( 21,270,80,0,0,100,100 );  //  太陽
		for( iText=0;  iText<4;  iText++ )
		{
			BltFastRect( 10,TextBltPosX[iText],180,TextSrcPosX[Rand(4)],0,120,120 );  //  文字
		}
		
		
		//  【 キー入力時挙動 】
		if( IsPushEscKey() ){
			return(-1);
			break;  //  [ESC]キーで終了
		}
		
		
		//  【 Flipせよ 】
		halt;
	}
	
	
	//  ＜03＞  「Go!」
	PlayWave(14);
	
	
	//  ＜04＞  ゲーム最中
	frameCnt = 0;
	
	for( timeCnt=0;  timeCnt<1200;  timeCnt++ )
	{
		//  【 フレーム初期化 】
		ClearSecondary();
		KeyInput();
		
		
		//  【 問題作成 】
		if( TextNo == 4 )
		{
			RndTblIndex[0] = Rand(4);
			RndTblIndex[1] = Rand(3);
			RndTblIndex[2] = Rand(2);
			RndTblIndex[3] = !(RndTblIndex[2]);
			
			TextOrder[0] = TextArray_0[ RndTblIndex[0] ];  //  １文字目
			TextOrder[1] = TextArray_1[ (RndTblIndex[0]*3) + RndTblIndex[1] ];  //  ２文字目
			TextOrder[2] = TextArray_2[ (RndTblIndex[0]*6) + (RndTblIndex[1]*2) + RndTblIndex[2] ];  //  ３文字目
			TextOrder[3] = TextArray_2[ (RndTblIndex[0]*6) + (RndTblIndex[1]*2) + RndTblIndex[3] ];  //  ４文字目
			
			for( iText=0;  iText<4;  iText++ )
			{
				TextType[iText] = Rand(UseKatakana+1);
			}
			
			StageNo++;
			TextNo = 0;
		}
		
		
		//  【 お手付きペナルティー中？ 】
		if( BooCnt )
		{
			//  お手付きペナルティー中
			
			BltFastRect(21,260+Rand(20),70+Rand(20),0,0,100,100);
			Blt(23,272,192);
			BooCnt--;
			
		}else{
			//  通常状態
			
			//  【 キー入力判定 】
			//  同時押しインチキ防止措置
			BtnValue = 0;
			for( iBtn=0;  iBtn<4;  iBtn++ )
			{
				if( IsPushKey( BtnCode[iBtn] ) ){
					BtnValue = BtnValue + BtnValueArray[iBtn];
				}
			}
			
			if( BtnValue )  //  何かしか押された。
			{
				if( BtnValue == BtnValueArray[ TextOrder[TextNo] ] ){
					//  正しく押された
					TextNo++;
					if(TextNo == 4){
						PlayWave(11);
					}else{
						PlayWave(10);
					}
					BtnPushNum++;
				}else{
					//  間違った or 同時押しインチキ
					PlayWave(12);
					BooCnt = 20;  //  お手付き
				}
			}
			
			
			//  【 描画 】
			for( iText=TextNo;  iText<4;  iText++ )
			{
				BltFastRect(
				10 , TextBltPosX[ TextOrder[iText] ] , 180 , 
				TextSrcPosX[ iText ] , TextSrcPosY[ TextType[iText] ] ,
				120 , 120
				);  //  文字の表示
			}
			BltFast(11,80,300);  //  ボタンラベル
			BltFastRect(21,270,80,0,(BtnPushNum % 6)*100,100,100);  //  太陽
			
		}
		
		
		//  【 キー入力時挙動 】
		if( IsPushEscKey() ){
			return(-1);
			break;  //  [ESC]キーで終了
		}
		
		
		//  【 Flipせよ 】
		halt;
	}
	
	
	//  ＜05＞  ゲーム終了（暫しの間）
	PlayWave(14);
	for( frameCnt=0;  frameCnt<20;  frameCnt++ )
	{
		//  【 フレーム初期化 】
		ClearSecondary();
		KeyInput();
		
		
		//  【 Flipせよ 】
		halt;
	}
	
	
	//  ＜06＞  結果発表
	
	//  め、面倒臭い……。
	TextSize(10,64);
	TextColor(10,255,0,0);
	TextBackColorDisable(10);
	TextLayerOn(10,0,200);
	sprintf1(string[10],"さいたま指数：%d",BtnPushNum);
	TextOut(10,string[10]);
	
	loop
	{
		//  【 フレーム初期化 】
		ClearSecondary();
		KeyInput();
		
		
		//  【 キー入力時挙動 】
		if( IsPushReturnKey() ){
			ResValue = 0;
			break;  //  [Enter]キーでタイトルに戻る
		}
		if( IsPushEscKey() ){
			return(-1);
			//break;  //  [ESC]キーで終了
		}
		
		
		//  【 Flipせよ 】
		halt;
	}
	
	TextLayerOff(10);
	
	
	return( ResValue );
}



/*  ■ メイン関数 ■  */
int main(void)
{
	//  初期化
	Init();
	
	//  画像・音声の読み込み
	LoadObject();
	
	
	//  メインなループ
	loop
	{
		//  タイトル画面
		if( sceneTitle() ){
			break;
		}
		
		//  ゲーム画面
		if( sceneGameMain() ){
			break;
		}
	}
	
	
	//  画像・音声の解放
	ReleaseObject();
	
	
	return(0);
}



/*_EOF_*/