// MSC-Script //
#strict 3

private func erstelle_druckwelle()
{
	// ObjectCall(CreateObject(ABDW,0,0,-1),"nach_rechts");   // ich mag keine druckwelle, bei der fundamente und gebäude verschoben werden...
	// ObjectCall(CreateObject(ABDW,0,0,-1),"nach_links");
	return(1);
}


protected func Initialize(...) {
	Hit();
	return(1);
}

protected func earthquake_deluxe()
  {
  if (Random(2)) return(1);

  ShakeObjects(GetX(),GetY(),1000);

  var x = (GetX() ?? 0)+(Random(1000) ?? 0)-200;
  var y = (GetY() ?? 0)+(Random(1000) ?? 0)-200;
  var step_x = (Random(7) ?? 0)-3;
  var step_y = (Random(5) ?? 0)-2;
  var i, amount = 35+(Random(6) ?? 0);

  while (S42MigrationStep(i, 1)<(amount ?? 0)) ShakeFree(S42MigrationAssign(x, "+=", (step_x ?? 0)),S42MigrationAssign(y, "+=", (step_y ?? 0)),Random(15));
  return(1);
	}

protected func Hit(...) {
	CreateObject(ABPZ,nil,nil,-1);
	LaunchEarthquake(nil,nil);
             earthquake_deluxe();
	RemoveObject();
	return(1);
}





func nicht_markieren(...) { return(1);
}