/*-- Blitz --*/

#strict 3

/* Locals */

local iAdvX, iVarX, // Bewegung / Abweichung X
      iAdvY, iVarY; // Bewegung / Abweichung Y

/* Aktivierung */

public func Activate(iStartX, iStartY, iAdvanceX, iRandomX, iAdvanceY, iRandomY)
{
  // Startposition
  AddVertex(iStartX,iStartY);
  // Bewegungsverhalten
  iAdvX=iAdvanceX; iVarX=iRandomX;
  iAdvY=iAdvanceY; iVarY=iRandomY;
  // Aktion setzen
  SetAction("Advance");
  // Geräusch
  if (!Random(5)) Sound("Thunder*");
  return(1);
}

public func Launch(iX, iY, iAngle)
{
  SetVisibility(VIS_None);
  var XDir;
  XDir = Cos(iAngle, 10, 10);
//  if (iDir == DIR_Left()) XDir = -Cos(iAngle, 10, 10);
//  else (XDir = +Cos(iAngle, 10, 10));
  Activate(iX, iY, XDir, nil, -(Sin(iAngle, 10, 10) ?? 0));
  return(1);
}

private func Timer()
{
  var i=nil;
  var r;
  // Partikel-Effekt
  while((i ?? 0) < (GetVertexNum() ?? 0)-1)
  {
        r = (90*(i ?? 0))/(GetVertexNum() ?? 0);
    DrawParticleLine ("LightningSpark", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0), 6, 60, RGB(255,255,128+(Sin(r,255) ?? 0)));
    DrawParticleLine ("LightningSpark", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0),  3, 25, RGB(255,255,250));
    S42MigrationStep(i, 1, true);
  }
}

/* Bewegung */

private func Advance()
{
  // Einschlag
  var iVtx = (GetVertexNum() ?? 0)-1, iVtxX = GetVertex(iVtx, nil), iVtxY = GetVertex(iVtx, 1);
  if (GBackSolid((iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0) ))
    return(Remove());

  // Neuen Eckpunkt
  if (!AddVertex( (iVtxX ?? 0)+(iAdvX ?? 0)+(RandomX(-5,5) ?? 0), (iVtxY ?? 0)+(iAdvY ?? 0)+(RandomX(-5,5) ?? 0)))
    return(Remove());

  SetPosition(GetVertex(GetVertexNum()), GetVertex(GetVertexNum(), 1));
  
  // Im Wasser?
  if (GBackLiquid((iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0) ))
  {
	  var obj;
	  // Alle Lebewesen in der Nähe bekommen was ab
	  while (obj = FindObject( nil, (iVtxX ?? 0)-(GetX() ?? 0)-400, (iVtxY ?? 0)-(GetY() ?? 0)-400, 800, 800, OCF_Alive, nil,nil, NoContainer(),obj  ) )
    	  {
      	    if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
	    if(!GBackLiquid((GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0))) continue;
  	    obj->~LightningStrike();
  	    DoDmg(12, DMG_Energy, obj);
            DrawParticleLine ("LightningSpark2", (iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0),
		    (GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0), 6, 60, RGB(nil,100,255));
            DrawParticleLine ("LightningSpark2", (iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0),
		    (GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0),  3, 25, RGB(250,250,255));
          }
	  return(Remove());
  }
  
  // Objektanziehung
  iVtx = (GetVertexNum() ?? 0)-1; iVtxX = GetVertex(iVtx, nil); iVtxY = GetVertex(iVtx, 1);
  var obj;
  if ((iVtx ?? 0)>7)
  {
    while (obj = FindObject( nil, (iVtxX ?? 0)-(GetX() ?? 0)-25, (iVtxY ?? 0)-(GetY() ?? 0)-25, 50, 50, OCF_Alive, nil,nil, NoContainer(),obj  ) )
    {
      if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
      Attraction(obj);
      break;
    }
  }
  for(obj in FindObjects(Find_ID(GetID()), Find_Distance(35,(iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0)), Find_Exclude(this())))
  {
    AttractionLightning(obj);
    break;
  }
  
//  Timer();
  // Weiter	
  return;
}

/* Effekte */

private func Sparkle()
{
//  Timer();
  var iVtx = (GetVertexNum() ?? 0)-1, iVtxX = GetVertex(iVtx, nil), iVtxY = GetVertex(iVtx, 1);
  CastObjects(SPRK,1,20,(iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0));
  return(1);
}

/* Anziehung */

private func Attraction(obj)
{
  // Einschlag in Objekt
  AddVertex( (GetVertex(nil,nil,obj) ?? 0)+(GetX(obj) ?? 0),
             (GetVertex(nil,1,obj) ?? 0)+(GetY(obj) ?? 0) );
  obj->~LightningStrike(this());
  SetAction("Connect");
  DoDmg(20, DMG_Energy, obj);
}

public func LightningStrike(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex((GetVertexNum(obj) ?? 0)-1,nil,obj),
             GetVertex((GetVertexNum(obj) ?? 0)-1,1,obj) );
  SetAction("Connect");
}

private func AttractionLightning(obj)
{
  // Einschlag in Objekt
  AddVertex( GetVertex((GetVertexNum(obj) ?? 0)-1,nil,obj),
             GetVertex((GetVertexNum(obj) ?? 0)-1,1,obj) );
  obj->~LightningStrike(this());
  SetAction("Connect");
}

/* Ende */

private func Remove()
{
  var i=nil;
  var r;
  // Leuchten
  while((i ?? 0)<(GetVertexNum() ?? 0)-1)
  {
   r = (180*(i ?? 0))/(GetVertexNum() ?? 0);
    DrawParticleLine ("LightningSpark", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0), 6, 60-(r ?? 0)/6, RGBa(255,255,128+(Sin(r,128) ?? 0),20));
    DrawParticleLine ("LightningSpark", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0),  2, 25-(r ?? 0)/10, RGB(255,255,250));
		    
    DrawParticleLine ("LightningSpark2", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0), 6, 60-(r ?? 0)/6, RGBa(255,255,128+(Sin(r,128) ?? 0),20));
    DrawParticleLine ("LightningSpark2", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0),  2, 25-(r ?? 0)/10, RGB(255,255,250));
    S42MigrationStep(i, 1, true);
  }
  
  RemoveObject();
}
