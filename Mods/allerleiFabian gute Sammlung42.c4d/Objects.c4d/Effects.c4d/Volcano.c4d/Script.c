/* Vulkan */
#strict 3

local iWidth,iMat, iXDir;
local iStrength, iLiveTime;
local OldX, OldY;

/* Ausbruch */ 

func Advance() {
  var obj, advancex=(Random(11) ?? 0)-5+(iXDir ?? 0), advancey=(Random(8) ?? 0)-12;
	var xdir, ydir;
	// Verzweigungen
	if(!Random(30)) Branch();
	
  if(!InGround()) {
		SetAction("Discharge");
		/*for(var i=0; i<Max(3,iWidth/3); i++) 
		  CreateParticle("LavaGlow",GetX()-iWidth/2+Random(iWidth),GetY()+5-Random(20),0,0,100+Random(50+2*iWidth),RGBa(255,255,255,50));*/
			xdir=30-(Random(60) ?? 0);
			ydir=-20-(Random(50) ?? 0)-(iWidth ?? 0)/2;
	} else {
		xdir=((GetX() ?? 0)-(OldX ?? 0))*10;
		ydir=((GetY() ?? 0)-(OldY ?? 0))*10;
	}
	// Objekte mitschleudern
	while(obj=FindObject(nil,(Max(OldX,GetX()) ?? 0)-(Min(OldX,GetX()) ?? 0)-(iWidth ?? 0)/2,nil,(iWidth ?? 0)+(Abs((OldX ?? 0)-(GetX() ?? 0)) ?? 0),(OldY ?? 0)-(GetY() ?? 0),OCF_Collectible,nil,nil,nil,obj)) {
		SetYDir(ydir, obj);
	  SetXDir(xdir, obj);
	}
	
  OldX=GetX();
  OldY=GetY();
  SetPosition((GetX() ?? 0)+(advancex ?? 0),(GetY() ?? 0)+(advancey ?? 0));
	if((iWidth ?? 0)>5)
		if(!Random(3)) Sound("Lava*");
	
	if((iWidth ?? 0)>10) {
		//CreateParticle("LavaGlow",-iWidth/2+Random(iWidth),+5-Random(20),0,0,100+Random(50+2*iWidth),RGBa(255,255,255,50));
		if(!Random(10)) ShakeViewPort(10+(Random(iWidth) ?? 0),this());
	}
  DrawLine(OldX,OldY,GetX(),GetY());
  DrawMaterialQuad(Format("%s-Flat",MaterialName(iMat)),
    (GetX() ?? 0)-(iWidth ?? 0)/2, GetY(),(GetX() ?? 0)+(iWidth ?? 0)/2, GetY(),
    (OldX ?? 0)+(iWidth ?? 0)/2,OldY,(OldX ?? 0)-(iWidth ?? 0)/2,OldY,1);
	
  if(!InBounds()) return(Remove());
  DecreaseSize();
}

/* Zeichenfunktionen f¸r die Kokelerde */

func DrawLine(x1, y1, x2, y2) {
  var colr, coll,x,y, i,dist=Distance(x1,y1,x2,y2);
	var wdt, max=90; // max. Ankokelung
  i=dist;
  while((i ?? 0)>=0) {
    x=(x2 ?? 0)+(((x1 ?? 0)-(x2 ?? 0))*(i ?? 0))/(dist ?? 0)-(GetX() ?? 0);
    y=(y2 ?? 0)+(((y1 ?? 0)-(y2 ?? 0))*(i ?? 0))/(dist ?? 0)-(GetY() ?? 0);
    if(GBackSolid((x ?? 0)+(iWidth ?? 0)/2+2,y)) {
			wdt=Min(1+(iWidth ?? 0),3+(Random(4) ?? 0));
			colr=GetMaterialColorX(GetMaterial((x ?? 0)+(iWidth ?? 0)/2+2,y),nil);
			DrawXGradient((x ?? 0)+(iWidth ?? 0)/2,y,colr,wdt,max,1);
    }
    if(GBackSolid((x ?? 0)-(iWidth ?? 0)/2-2,y)) {
			wdt=Min(1+(iWidth ?? 0),3+(Random(4) ?? 0));
			coll=GetMaterialColorX(GetMaterial((x ?? 0)-(iWidth ?? 0)/2-2,y),nil);
			DrawXGradient((x ?? 0)-(iWidth ?? 0)/2,y,coll,wdt,max,nil);
    }
    S42MigrationStep(i, -1, true);
  }
}

func DrawXGradient(x, y, col, wdt, max, dir) {
	dir=-1+2*(dir ?? 0);
	var fact=(max ?? 0)/(wdt ?? 0);
	for(var i=1; (i ?? 0)<=(wdt ?? 0); S42MigrationStep(i, 1, true)) SetLandscapePixel((x ?? 0)+(i ?? 0)*(dir ?? 0),y,DarkenRGB((max ?? 0)-(i ?? 0)*(fact ?? 0),col));
}

func DarkenRGB(amm,val) {
  return(LightenRGB(-(amm ?? 0),val));
}

func LightenRGB(amm, val) {
  var red=GetRGBValue(val, 1), green=GetRGBValue(val,2),blue=GetRGBValue(val,3);
  val=SetRGBaValue(val,BoundBy((red ?? 0)+((amm ?? 0)*(red ?? 0))/100,nil,255),1);
  val=SetRGBaValue(val,BoundBy((green ?? 0)+((amm ?? 0)*(green ?? 0))/100,nil,255),2);
  val=SetRGBaValue(val,BoundBy((blue ?? 0)+((amm ?? 0)*(blue ?? 0))/100,nil,255),3);
  return(val);
}

/* Eruption */ 

func CastLava() {
  // Hoehle die sich mit Lava vollfuellt?
  if(InGround()) {
    S42MigrationAssign(iLiveTime, "-=", (GetActTime() ?? 0));
    return(SetAction("Advance"));
  }
  // Vulkane lassen alle Fl¸ssigkeiten verdampfen
  if(And(And(GBackLiquid(nil,-2), !Equal(GetMaterial(nil,-2), Material("Lava"))), !Equal(GetMaterial(nil,-2), Material("DuroLava"))))
   for(var i; (i ?? 0)<5; S42MigrationStep(i, 1, true)) ExtractLiquid(nil,-2);
  // dicke Rauchschwaden
  for(var i=1+(Random(3) ?? 0); (i ?? 0)>=0; S42MigrationStep(i, -1, true)) {
		Smoke(-(iWidth ?? 0) / 2 + (Random(iWidth) ?? 0), +15, 10 + (Random((iWidth ?? 0) / 2) ?? 0), HSL2RGB(RGB(38, 200, 100)));
		//if(!Random(3)) Smoke(-iWidth/2+Random(iWidth), +15, 3+Random(4),HSL2RGB(RGB(38,200+Random(55),100))); 
	}
  // ggf. hoeher casten
  while(And(GBackSolid(nil,nil), GetY())) SetPosition(GetX(),(GetY() ?? 0)-1);
  // rausrinnen
  for(var i=3+(Random(5) ?? 0); (i ?? 0)>0; S42MigrationStep(i, -1, true)) InsertMaterial(iMat,nil,nil);
  // rumspritzen
  if (!Random(2))
  {
  	var y_diff;
        while(Equal(GetMaterial(nil,y_diff), iMat)) S42MigrationStep(y_diff, -1, true);
        CastPXSX(MaterialName(iMat), Max((iWidth ?? 0)/2,5), (Max(50, iWidth) ?? 0) + (Random(50) ?? 0), -(iWidth ?? 0)/2 + (Random(iWidth) ?? 0), (y_diff ?? 0) - 1, 270 + 10 - (Random(21) ?? 0), 20 + (Random(30) ?? 0));
  }  
  // mit Lavaklumpen um sich werfen
  if(Or(!!SEqual(MaterialName(iMat), "Lava"), !!SEqual(MaterialName(iMat), "DuroLava")))
    if((iWidth ?? 0)>10)
      if(!Random(5)) {
        CastObjectsX(LAVA,1,(Max(40,iWidth) ?? 0)+(Random(30) ?? 0),-(iWidth ?? 0)/2+(Random(iWidth) ?? 0),nil,270,50,"LavaCast");
				Sound("Discharge");
				if(!Random(5))	ShakeViewPort(10+(Random(20) ?? 0)+(Random(iWidth) ?? 0),this());
      }
  if((GetActTime() ?? 0)>(iLiveTime ?? 0)) {
    //Log("my time is running out aarrgh...........*plop*");
    Remove(); 
  }
}

/* Callback of CastObjectsX */
func LavaCast(obj) {
  LocalN("mat",obj)=MaterialName(iMat);
  DoCon(-50+(Random(5*(iWidth ?? 0)) ?? 0)+(iWidth ?? 0)/2,obj);
}

/* Aktivierung des Vulkans */
func Activate(iX,iY,iStr,iM,iXD,iTime) {
  AddVertex(iX,iY);
  iWidth=iStr;
	if(!(iMat=GameCall("VolcanoMaterial"))) iMat=iM;
  iXDir=iXD;
	SetPosition(iX,iY);
  if(iTime) iLiveTime=iTime;
  else iLiveTime=200+2*(iWidth ?? 0);
  if(!InGround()) return(Remove());
  SetAction("Advance");
  if(!Random(5)) Sound(Format("%s*",MaterialName(iMat)));
  // unteres Ende abrunden
  DrawMaterialQuad(Format("%s-Flat",MaterialName(iMat)),
    (iX ?? 0)-(iWidth ?? 0)/2,iY,
    (iX ?? 0)+(iWidth ?? 0)/2,iY,
    (iX ?? 0)+(Random((iWidth ?? 0)/2) ?? 0),(iY ?? 0)+(iWidth ?? 0)/4+(Random((iWidth ?? 0)/4) ?? 0),
    (iX ?? 0)-(Random((iWidth ?? 0)/2) ?? 0),(iY ?? 0)+(iWidth ?? 0)/4+(Random((iWidth ?? 0)/4) ?? 0),
    1);
  return(1);
}

/* Vulkanader abspalten */

private func Branch() {
  var pVolcano=CreateObject(FXV1,nil,nil,-1);
  var iStr=Random(iWidth);
  var iXDir=2+(Random(4) ?? 0);
  iXDir=(1-2*(Random(2) ?? 0))*(iXDir ?? 0);
  pVolcano->FXV1::Activate(-(iWidth ?? 0)/2+(GetX() ?? 0)+(iStr ?? 0)/2,GetY(),iStr,iMat,iXDir);
  //SetVertex(iLastVertex,0,GetVertex(iLastVertex,0)+iStr/2);
  S42MigrationAssign(iWidth, "-=", (iStr ?? 0));
}

/* Vulkanader verkleinern */

private func DecreaseSize() {
  if(Random(5)) return;
  if(S42MigrationStep(iWidth, -1)<=0) Remove();
}


/* Pr¸fungen */

private func InGround() {
  // letzter Vertex in festem Material -> im Untergrund
  if (GBackSolid(nil,-8)) return(1);
  // letzter Vertex im Material was gecastet wird -> im Untergrund
  if (Or(Equal(GetMaterial(nil,-8), iMat), (And(Equal(iMat, Material("Lava")), Equal(GetMaterial(nil,-8), Material("DuroLava")))))) return(1);
  // ansonsten nicht im Untergrund
  return(nil);
}

private func InBounds() {
  // auﬂerhalb der Landschaft
  if((GetX() ?? 0)+(iWidth ?? 0)/2<0) return(nil);
  if((GetX() ?? 0)-(iWidth ?? 0)/2>(LandscapeWidth() ?? 0)) return(nil);
  if (!Inside(GetY(),nil,LandscapeHeight())) return(nil);
  // nicht auﬂerhalb der Landschaft
  return(1);
}

private func Remove() { 
  RemoveObject(); 
}