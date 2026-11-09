#strict

protected func Activate(pClonk)
{
  var Typ,i;
  Message("$TxtObject$",this());
  CreateMenu(GetID(),pClonk,this(),0,"$TxtEmptyBook$",0,0,1);
  AddMenuItem("Die Umweltstufen","Desc",_BAL,pClonk,0,pClonk);
  AddMenuItem("Umwelt allgemein","Desc",_BAO,pClonk,0,pClonk);
  AddMenuItem("Temperatur","Desc",CL22,pClonk,0,pClonk);
  AddMenuItem("Dunkelheit","Desc",TS9Z,pClonk,0,pClonk);
}

private func Desc(idItem,pClonk)
{
  if(idItem == _BAL) {
  MessageWindow("Es gibt 6 Umwelt Zustände, je nach dem, wieviel Prozent der Höchtbelastbarkeit erreicht ist. | Stufe 0, 0% bis 40%. | Stufe 1, 40% bis 50% | Stufe 2, 50% bis 60% | Stufe 3, 60% bis 70% | Stufe 4, 70% bis 99% | Stufe 5,   ab 100% | Welche Auswirkung jede einzelne Stufe hat, wirst du schon früh genug merken, sie werde bei Erreichen der Stufe aber auch angesagt. Es wird aber natürlich nicht gerade einfacher, wenn die Umwelt unter dir leidet.", GetOwner(pClonk));
  return(1);
}

  if(idItem == _BAO) {  MessageWindow("Je nach Einstellungen/ Forschungsstand gibt es eine Höchstzahl für die Umweltbelastbarkeit. Besonders Herstellungsprozesse verschmutzen durch giftige Abgase und Müll. Du kannst den aktuellen Verschmutzungsgrad in folgendenen Gebäuden mit der graben Taste einsehen: Forschungsgebäude, Müllverbrennungsgebäude, Luftregenerator. Jeder ausgewachsene Baum filtert die Umwelt und gleicht damit 5 Schadenspunkte aus. Jeder tote Baum gleicht nur 1 Schadenspunkt aus und jedes Stück Gras immerhin 1/4. Du kannst Müll vergraben oder verbrennen.", GetOwner(pClonk));
  return(1);}
  
  if(idItem == CL22) {  MessageWindow("Wenn tödliche Temperaturen aktiviert ist, bekommen deine Clonks bei zu heißen oder zu kalten Temperaturen Schaden. Kurz bevor es soweit ist, erscheint oben links eine Warnmeldung. Die Temperatur zeigt dir z.b dein Forschungslabor an, wenn du die graben Taste drückst. In Gebäuden bist du sicher. In der Nähe von Wasser oder einem Wasserfass bist du vor Hitze, in der Nähe von Feuer vor Kälte geschützt. Ein später erforschbarer Schutzanzug und Lüftungsventilatoren schützen vor Beidem.", GetOwner(pClonk));
  return(1);}
  
  if(idItem == TS9Z) {  MessageWindow("Nachts kann es sehr dunkel werden. Auch unter der Erde kann man selten weit sehen. Hierbei helfen alle möglichen Lichtobjekte, wie Fackeln, Lagerfeuer, Standleuchten, Deckenlampen, Taschenlampen und Scheinwerfer. Sie bringen Licht ins Dunkle und vergrößern deinen Sichtbereich. In den Regeln kann sowohl das Sichtverhalten, als auch ob Helligkeitswechsel stattfinden sollen oder nicht, eingestellt werden.", GetOwner(pClonk));
  return(1);}
}