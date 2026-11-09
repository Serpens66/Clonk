/*-- Autobau --*/

#strict 3


func Initialize() {
   
  return(1);
}


func Activate(int iPlayer)
{
	iPlayer ??= 0;
	MessageWindow("Clonks bauen Gebäude sofort fertig!",iPlayer);
}

// im clonkskript selbst unter func building geregelt!