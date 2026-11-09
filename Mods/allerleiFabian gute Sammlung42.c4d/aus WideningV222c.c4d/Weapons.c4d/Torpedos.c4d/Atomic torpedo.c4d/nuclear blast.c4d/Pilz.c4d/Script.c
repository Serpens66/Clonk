// MSC-Script //
#strict 3



	
protected func Initialize(...) {
	Sound("NukeBlast");
	DoCon(-90,CreateObject(ABP1,nil,-50,-1));
	CreateObject(ABFO,nil,nil,-1);
	local yc;
	yc=10;
	// ObjectCall(CreateObject(ABDW,0,0,-1),"nach_rechts");  // ich mag keine druckwelle
	// ObjectCall(CreateObject(ABDW,0,0,-1),"nach_links");
    
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);
	Smoke((Random(80) ?? 0)-40,(Random(80) ?? 0)-40,100);


	if(!FindObject(VSCH))
    {
        ObjectCall(CreateObject(ABSR,150,(Random(-50) ?? 0)+(yc ?? 0),-1),"nach_rechts");
        //ObjectCall(CreateObject(ABSR,130,Random(-45)+yc,-1),"nach_rechts");
        ObjectCall(CreateObject(ABSR,110,(Random(-45) ?? 0)+(yc ?? 0),-1),"nach_rechts");
        //ObjectCall(CreateObject(ABSR,90,Random(-40)+yc,-1),"nach_rechts");
        ObjectCall(CreateObject(ABSR,70,(Random(-40) ?? 0)+(yc ?? 0),-1),"nach_rechts");
        //ObjectCall(CreateObject(ABSR,60,Random(-35)+yc,-1),"nach_rechts");
        ObjectCall(CreateObject(ABSR,50,(Random(-35) ?? 0)+(yc ?? 0),-1),"nach_rechts");
        //ObjectCall(CreateObject(ABSR,40,Random(-35)+yc,-1),"nach_rechts");
        ObjectCall(CreateObject(ABSR,30,(Random(-30) ?? 0)+(yc ?? 0),-1),"nach_rechts");
        //ObjectCall(CreateObject(ABSR,20,Random(-30)+yc,-1),"nach_rechts");
        ObjectCall(CreateObject(ABSR,10,(Random(-30) ?? 0)+(yc ?? 0),-1),"nach_rechts");
        
        //ObjectCall(CreateObject(ABSR,20,Random(10)+20+yc,-1),"static");
        ObjectCall(CreateObject(ABSR,40,(Random(10) ?? 0)+20+(yc ?? 0),-1),"static");
        //ObjectCall(CreateObject(ABSR,-20,Random(10)+20+yc,-1),"static");
        ObjectCall(CreateObject(ABSR,-40,(Random(10) ?? 0)+20+(yc ?? 0),-1),"static");
        //ObjectCall(CreateObject(ABSR,-1,Random(20)-10+yc,-1),"static");
        
        ObjectCall(CreateObject(ABSR,-150,(Random(-50) ?? 0)+(yc ?? 0),-1),"nach_links");
        //ObjectCall(CreateObject(ABSR,-120,Random(-45)+yc,-1),"nach_links");
        ObjectCall(CreateObject(ABSR,-110,(Random(-45) ?? 0)+(yc ?? 0),-1),"nach_links");
        //ObjectCall(CreateObject(ABSR,-90,Random(-40)+yc,-1),"nach_links");
        ObjectCall(CreateObject(ABSR,-70,(Random(-40) ?? 0)+(yc ?? 0),-1),"nach_links");
        //ObjectCall(CreateObject(ABSR,-60,Random(-35)+yc,-1),"nach_links");
        ObjectCall(CreateObject(ABSR,-50,(Random(-35) ?? 0)+(yc ?? 0),-1),"nach_links");
        //ObjectCall(CreateObject(ABSR,-40,Random(-30)+yc,-1),"nach_links");
        ObjectCall(CreateObject(ABSR,-30,(Random(-30) ?? 0)+(yc ?? 0),-1),"nach_links");
        //ObjectCall(CreateObject(ABSR,-20,Random(50)+yc,-1),"nach_links");
        ObjectCall(CreateObject(ABSR,-10,(Random(50) ?? 0)+(yc ?? 0),-1),"nach_links");
    }
	




	CreateObject(ABPD,-50,-10,-1);
	CreateObject(ABPD,nil,nil,-1);
	CreateObject(ABPD,50,-10,-1);

	if(!FindObject(VSCH)) CastObjects(ABRW,25,70,nil,-100); // wenn Regel "kein verschmutzungsnbel" aktiv ist, dann keine wolken, die sind eh nur noch effekt

	SetClimate(-50);
	SetTemperature(-50);
    
    var obj = CreateObject(SWAV);   // erstelle die space empire druckwelle,welche aus EMP auslöst
    ObjectCall(obj, "SetSize", 300,GetX(),GetY(), 500+(Random(200) ?? 0), 800+(Random(400) ?? 0));

	NewGamma(RGB(255,255,255),RGB(255,255,255),RGB(255,255,255),nil,300);
	NewGamma(RGB(nil,nil,nil),RGB(128,128,128),RGB(255,255,255),nil,60);

	RemoveObject();
	return(1);
}

func nicht_markieren(...) { return(1);
}