//	gamestart.c
/*
	�������܃Q�[��(��)
	�R�A���W���[��
	Coded by Kolorz.
	( 02/10/27 )
*/


/*  �� Config�I���� ��  */
int UseKatakana = 0;  //  �J�^�J�i���g�p�i0:��g�p  1:�g�p�j



/*  �� �O���[�o���ϐ� ��  */
int gScreenMode;

//  �����̕��я�
int TextArray_0[ 4] = {  //  �P������
	0,1,2,3
};
int TextArray_1[12] = {  //  �Q������
	1,2,3,
	0,2,3,
	0,1,3,
	0,1,2
};
int TextArray_2[24] = {  //  �R�����ځ��S������
	2,3,  1,3,  1,2,
	2,3,  0,3,  0,2,
	1,3,  0,3,  0,1,
	1,2,  0,2,  0,1
};



/*  �� ���̃Q�[���S�̂̏����� ��  */
void Init(void)
{
	SetSystemMemoryUse(0);		//  �r�f�I�������Ń����V�N
	SetFillColor(0xffffffff);	//  �h��ׂ��F�͔���
	
	gScreenMode = 0;
	
	//  ���ʃp���b�g�̐ݒ�
	LoadBitmap("./system/pallete.bmp",0,1);
	ReleaseBitmap(0);
}



/*  �� �摜�E�����̓ǂݍ��� ��  */
void LoadObject(void)
{
	/*
		[ PlaneObject(s). ]
		10 : ����(text.bmp)
		11 : �{�^������(label.bmp)
		20 : �^�C�g�����S(title.bmp)
		21 : ���z(sun.bmp)
		22 : �������ܒB(stmchar.bmp)
		23 : �~�}�[�N(boo.bmp)
		
		[ SoundObject(s). ]
		10 : �u�������܁I�v(stm0.wav)
		11 : �u�������܁`�v(stm1.wav)
		12 : �u�ځ`��v(boo.wav)
		13 : �uready?�v(ready.wav)
		14 : �u�������܁J���J���J�v(go.wav)
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



/*  �� �摜�E�����̉�� ��  */
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



/*  �� �t���X�N���[������Window���[�h�̐؂�ւ� ��  */
void SwitchScreenMode(void)
{
	gScreenMode = !(gScreenMode);
	ChangeDisplayMode( gScreenMode+1 );
}



/*  ��  �V�[���؂�ւ����̉��o(�H)����  ��  */
void ToNextScene(int fncNextScene)
{
	//  ������
	//  �@������
	
	fncNextScene();  //  ���̃V�[�����R�[��
}



/*  �� �^�C�g����� ��  */
int sceneTitle(void)
{
	//  �ϐ���`
	int ResValue;
	int frameCnt;
	
	
	//  �V�[��������
	SetFPS(40);
	SetFillColor(0xffffffff);
	ClearSecondary();
	ResValue = 0;
	
	
	frameCnt = 0;
	loop
	{
		//  �y �t���[�������� �z
		ClearSecondary();
		KeyInput();
		
		
		//  �y �`�� �z
		BltFastRect(21,270,100,0,(frameCnt / 3 % 6)*100,100,100);
		BltFastRect(22,100,230+Rand(8),0,0,280,80);
		BltFastRect(22,320,200+Rand(8),0,80,280,110);
		Blt(20,120,(frameCnt%576)-48);
		
		
		//  �y ���� �z
		if(!(frameCnt % 15)){
			PlayWave(10);
		}
		
		
		//  �y �L�[���͎����� �z
		if( IsPushReturnKey() ){
			ResValue = 0;
			break;  //  [Enter]�L�[�ŃQ�[���J�n
		}
		if( IsPushEscKey() ){
			ResValue = -1;
			break;  //  [ESC]�L�[�ŏI��
		}
		if( IsPushKey(0x3E) ){
			SwitchScreenMode();  //  [F4]�L�[�ŃX�N���[�����[�h�؂�ւ�
		}
		
		
		//  �y Flip���� �z
		halt;
		
		frameCnt++;
	}
	
	
	return( ResValue );
}



/*  �� �Q�[����� ��  */
int sceneGameMain(void)
{
	//  �ϐ���`
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
	
	
	//  �V�[��������
	SetFPS(40);
	SetFillColor(0xffffffff);
	ClearSecondary();
	ResValue = 0;
	
	BtnValueArray[0] = 1;  //  0001
	BtnValueArray[1] = 2;  //  0010
	BtnValueArray[2] = 4;  //  0100
	BtnValueArray[3] = 8;  //  1000
	
	BtnCode[0] = 0x2C;  //  [Z]�L�[
	BtnCode[1] = 0x2D;  //  [X]�L�[
	BtnCode[2] = 0x2E;  //  [C]�L�[
	BtnCode[3] = 0x2F;  //  [V]�L�[
	
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
	TextNo = 4;  //  �������K�v����
	BooCnt = 0;
	BtnPushNum = 0;
	
	
	//  ��01��  �uReady?�v���
	PlayWave(13);
	
	for( frameCnt=0;  frameCnt<30;  frameCnt++ )
	{
		//  �y �t���[�������� �z
		ClearSecondary();
		KeyInput();
		
		
		//  �y �`�� �z
		BltFastRect( 21,270,(frameCnt*3)-10,0,0,100,100 );  //  ���z
		for( iText=0;  iText<4;  iText++ )
		{
			BltFastRect( 10,TextBltPosX[iText],180,TextSrcPosX[Rand(4)],0,120,120 );  //  ����
		}
		
		
		//  �y �L�[���͎����� �z
		if( IsPushEscKey() ){
			return(-1);
			break;  //  [ESC]�L�[�ŏI��
		}
		
		
		//  �y Flip���� �z
		halt;
	}
	
	
	//  ��02��  �uReady?�v�̌�̑҂�
	for( frameCnt=0;  frameCnt<6;  frameCnt++ )
	{
		//  �y �t���[�������� �z
		ClearSecondary();
		KeyInput();
		
		
		//  �y �`�� �z
		BltFastRect( 21,270,80,0,0,100,100 );  //  ���z
		for( iText=0;  iText<4;  iText++ )
		{
			BltFastRect( 10,TextBltPosX[iText],180,TextSrcPosX[Rand(4)],0,120,120 );  //  ����
		}
		
		
		//  �y �L�[���͎����� �z
		if( IsPushEscKey() ){
			return(-1);
			break;  //  [ESC]�L�[�ŏI��
		}
		
		
		//  �y Flip���� �z
		halt;
	}
	
	
	//  ��03��  �uGo!�v
	PlayWave(14);
	
	
	//  ��04��  �Q�[���Œ�
	frameCnt = 0;
	
	for( timeCnt=0;  timeCnt<1200;  timeCnt++ )
	{
		//  �y �t���[�������� �z
		ClearSecondary();
		KeyInput();
		
		
		//  �y ���쐬 �z
		if( TextNo == 4 )
		{
			RndTblIndex[0] = Rand(4);
			RndTblIndex[1] = Rand(3);
			RndTblIndex[2] = Rand(2);
			RndTblIndex[3] = !(RndTblIndex[2]);
			
			TextOrder[0] = TextArray_0[ RndTblIndex[0] ];  //  �P������
			TextOrder[1] = TextArray_1[ (RndTblIndex[0]*3) + RndTblIndex[1] ];  //  �Q������
			TextOrder[2] = TextArray_2[ (RndTblIndex[0]*6) + (RndTblIndex[1]*2) + RndTblIndex[2] ];  //  �R������
			TextOrder[3] = TextArray_2[ (RndTblIndex[0]*6) + (RndTblIndex[1]*2) + RndTblIndex[3] ];  //  �S������
			
			for( iText=0;  iText<4;  iText++ )
			{
				TextType[iText] = Rand(UseKatakana+1);
			}
			
			StageNo++;
			TextNo = 0;
		}
		
		
		//  �y ����t���y�i���e�B�[���H �z
		if( BooCnt )
		{
			//  ����t���y�i���e�B�[��
			
			BltFastRect(21,260+Rand(20),70+Rand(20),0,0,100,100);
			Blt(23,272,192);
			BooCnt--;
			
		}else{
			//  �ʏ���
			
			//  �y �L�[���͔��� �z
			//  ���������C���`�L�h�~�[�u
			BtnValue = 0;
			for( iBtn=0;  iBtn<4;  iBtn++ )
			{
				if( IsPushKey( BtnCode[iBtn] ) ){
					BtnValue = BtnValue + BtnValueArray[iBtn];
				}
			}
			
			if( BtnValue )  //  �������������ꂽ�B
			{
				if( BtnValue == BtnValueArray[ TextOrder[TextNo] ] ){
					//  �����������ꂽ
					TextNo++;
					if(TextNo == 4){
						PlayWave(11);
					}else{
						PlayWave(10);
					}
					BtnPushNum++;
				}else{
					//  �Ԉ���� or ���������C���`�L
					PlayWave(12);
					BooCnt = 20;  //  ����t��
				}
			}
			
			
			//  �y �`�� �z
			for( iText=TextNo;  iText<4;  iText++ )
			{
				BltFastRect(
				10 , TextBltPosX[ TextOrder[iText] ] , 180 , 
				TextSrcPosX[ iText ] , TextSrcPosY[ TextType[iText] ] ,
				120 , 120
				);  //  �����̕\��
			}
			BltFast(11,80,300);  //  �{�^�����x��
			BltFastRect(21,270,80,0,(BtnPushNum % 6)*100,100,100);  //  ���z
			
		}
		
		
		//  �y �L�[���͎����� �z
		if( IsPushEscKey() ){
			return(-1);
			break;  //  [ESC]�L�[�ŏI��
		}
		
		
		//  �y Flip���� �z
		halt;
	}
	
	
	//  ��05��  �Q�[���I���i�b���̊ԁj
	PlayWave(14);
	for( frameCnt=0;  frameCnt<20;  frameCnt++ )
	{
		//  �y �t���[�������� �z
		ClearSecondary();
		KeyInput();
		
		
		//  �y Flip���� �z
		halt;
	}
	
	
	//  ��06��  ���ʔ��\
	
	//  �߁A�ʓ|�L���c�c�B
	TextSize(10,64);
	TextColor(10,255,0,0);
	TextBackColorDisable(10);
	TextLayerOn(10,0,200);
	sprintf1(string[10],"�������܎w���F%d",BtnPushNum);
	TextOut(10,string[10]);
	
	loop
	{
		//  �y �t���[�������� �z
		ClearSecondary();
		KeyInput();
		
		
		//  �y �L�[���͎����� �z
		if( IsPushReturnKey() ){
			ResValue = 0;
			break;  //  [Enter]�L�[�Ń^�C�g���ɖ߂�
		}
		if( IsPushEscKey() ){
			return(-1);
			//break;  //  [ESC]�L�[�ŏI��
		}
		
		
		//  �y Flip���� �z
		halt;
	}
	
	TextLayerOff(10);
	
	
	return( ResValue );
}



/*  �� ���C���֐� ��  */
int main(void)
{
	//  ������
	Init();
	
	//  �摜�E�����̓ǂݍ���
	LoadObject();
	
	
	//  ���C���ȃ��[�v
	loop
	{
		//  �^�C�g�����
		if( sceneTitle() ){
			break;
		}
		
		//  �Q�[�����
		if( sceneGameMain() ){
			break;
		}
	}
	
	
	//  �摜�E�����̉��
	ReleaseObject();
	
	
	return(0);
}



/*_EOF_*/