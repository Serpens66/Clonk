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
  // Richtung setzen für korrektes Punch
  if ((iAdvX ?? 0) > 0) SetDir(DIR_Right); else SetDir(DIR_Left);
  // Geräusch
  if (!Random(5)) Sound("Thunder*");
  return(1);
}

public func Launch(iX, iY, iAngle)
{
  var XDir;
  XDir = Cos(iAngle, 10, 10);
//  if (iDir == DIR_Left) XDir = -Cos(iAngle, 10, 10);
//  else (XDir = +Cos(iAngle, 10, 10));
  Activate(iX, iY, XDir, nil, -(Sin(iAngle, 10, 10) ?? 0));
  return(1);
}

private func Timer()
{
  var i=nil;
  // Partikel-Effekt
  while((i ?? 0)<(GetVertexNum() ?? 0)-1)
  {
    DrawParticleLine ("LightningSpark", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0), 6, 60, RGB(nil,100,255));
    DrawParticleLine ("LightningSpark", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0),  3, 25, RGB(250,250,255));
    S42MigrationStep(i, 1, true);
  }
}

/* Bewegung */

private func Advance()
{
  var obj;
  
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
    // Alle Lebewesen in der Nähe bekommen was ab
    for (obj in FindObjects(Find_InRect((iVtxX ?? 0)-(GetX() ?? 0)-400, (iVtxY ?? 0)-(GetY() ?? 0)-400, 800, 800), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsForceField")), Find_NoContainer(), Find_Layer(GetObjectLayer()) ))
    {
      if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
      if(!GBackLiquid((GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0))) continue;
      // Selber so ausrichten, dass die Schlagrichtung stimmt
      var xdiff = (GetX(obj) ?? 0) - (GetVertex(Max((GetVertexNum() ?? 0)-3, nil), nil) ?? 0);
      if (!xdiff) xdiff = iAdvX;
      if ((xdiff ?? 0) > 0) SetDir(DIR_Right); else SetDir(DIR_Left);
      obj->~LightningStrike(this);
      Punch(obj, 12);
      DrawParticleLine ("LightningSpark2", (iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0),
           (GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0), 6, 60, RGB(nil,100,255));
      DrawParticleLine ("LightningSpark2", (iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0),
            (GetX(obj) ?? 0)-(GetX() ?? 0), (GetY(obj) ?? 0)-(GetY() ?? 0),  3, 25, RGB(250,250,255));
    }
    return(Remove());
  }
  
  // Objektanziehung
  iVtx = (GetVertexNum() ?? 0)-1; iVtxX = GetVertex(iVtx, nil); iVtxY = GetVertex(iVtx, 1);
  if ((iVtx ?? 0)>7)
  {
    for (var obj in FindObjects( Find_AtRect((iVtxX ?? 0)-(GetX() ?? 0)-13, (iVtxY ?? 0)-(GetY() ?? 0)-13, 26, 26), Find_Or(Find_OCF(OCF_Alive), Find_Func("IsForceField")), Find_Layer(GetObjectLayer()), Find_NoContainer() ) )
    {
      if(!PathFree(iVtxX, iVtxY, GetX(obj), GetY(obj))) continue;
      Attraction(obj);
      break;
    }
  }
  for(obj in FindObjects(Find_ID(GetID()), Find_Distance(35,(iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0)), Find_Layer(GetObjectLayer()), Find_Exclude(this())))
  {
    AttractionLightning(obj);
    break;
  }
  
  Timer();
  // Weiter	
  return;
}

/* Effekte */

private func Sparkle()
{
  Timer();
  var iVtx = (GetVertexNum() ?? 0)-1, iVtxX = GetVertex(iVtx, nil), iVtxY = GetVertex(iVtx, 1);
  CastObjects(SPRK,1,20,(iVtxX ?? 0)-(GetX() ?? 0), (iVtxY ?? 0)-(GetY() ?? 0));
  return true;
}

/* Anziehung */

private func Attraction(obj)
{
  // Einschlag in Objekt
  AddVertex( (GetVertex(nil,nil,obj) ?? 0)+(GetX(obj) ?? 0),
             (GetVertex(nil,1,obj) ?? 0)+(GetY(obj) ?? 0) );
  obj->~LightningStrike(this);
  SetAction("Connect");
  if (GetAlive(obj)) Punch(obj, 10);
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
  obj->~LightningStrike(this);
  SetAction("Connect");
}

/* Ende */

private func Remove()
{
  var i=nil;
  // Nachleuchten
  while((i ?? 0)<(GetVertexNum() ?? 0)-1)
  {
    DrawParticleLine ("LightningSpark2", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0), 6, 60, RGB(nil,100,255));
    DrawParticleLine ("LightningSpark2", (GetVertex(i) ?? 0)-(GetX() ?? 0), (GetVertex(i, 1) ?? 0)-(GetY() ?? 0),
		    (GetVertex((i ?? 0)+1) ?? 0)-(GetX() ?? 0), (GetVertex((i ?? 0)+1,1) ?? 0)-(GetY() ?? 0),  3, 25, RGB(250,250,255));
    S42MigrationStep(i, 1, true);
  }
  
  RemoveObject();
}
