/* ===========================================================
 *
 *       Filename:  key.h
 *
 *    Description:  
 *
 *        Created:  06-04-2013 22:31
 *
 *         Author:  Mael Valais 
 *         Mail  :	mael.valais@univ-tlse3.fr
 *
 * =========================================================== */

#ifndef __key_h__
#define __key_h__


void* keyCreer(int valeur);

void keySuppr(void* key);

int keyCmp(const void* key1, const void* key2);

int keyEqual(const void* key1, const void* key2);

int keyPut(void* key); /* XXX DEBUG */

#endif /* !__key_h__ */


