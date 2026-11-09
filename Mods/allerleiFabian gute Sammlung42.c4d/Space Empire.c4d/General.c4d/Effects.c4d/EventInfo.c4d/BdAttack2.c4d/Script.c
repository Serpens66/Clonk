/*-- Ereignis-Info --*/

#strict 3

#include SEVI

protected func Initialize()
{
  // nicht, wenn schon eins da
  if (FindObjectOwner(GetID(),GetOwner()))
    return(RemoveObject());
  return(inherited());
}

