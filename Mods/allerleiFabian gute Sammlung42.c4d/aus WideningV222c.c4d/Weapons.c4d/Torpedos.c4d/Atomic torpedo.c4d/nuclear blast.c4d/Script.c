// MSC-Script //
#strict

private func erstelle_druckwelle()
{
	// ObjectCall(CreateObject(ABDW,0,0,-1),"nach_rechts");   // ich mag keine druckwelle, bei der fundamente und gebäude verschoben werden...
	// ObjectCall(CreateObject(ABDW,0,0,-1),"nach_links");
	return(1);
}


protected Initialize:
	Hit();
	return(1);

protected func earthquake_deluxe()
  {
  if (Random(2)) return(1);

  ShakeObjects(GetX(),GetY(),1000);

  var x = GetX()+Random(1000)-200;
  var y = GetY()+Random(1000)-200;
  var step_x = Random(7)-3;
  var step_y = Random(5)-2;
  var i, amount = 35+Random(6);

  while (++i<amount) ShakeFree(x+=step_x,y+=step_y,Random(15));
  return(1);
	}

protected Hit:
	CreateObject(ABPZ,0,0,-1);
	LaunchEarthquake(0,0);
             earthquake_deluxe();
	RemoveObject();
	return(1);





nicht_markieren: return(1);