/*-- ZeitMultiplikator --*/

#strict

global func GetTimeMultiplicator()
{
  if(ObjectCount(SPDG))
   return 2;
  return 1;
}
