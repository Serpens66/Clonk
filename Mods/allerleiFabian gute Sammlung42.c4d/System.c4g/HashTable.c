/* HashTable.c
 * 
 * Copyright (c) 2008 Nicolas Hake
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.

Kurze Gebrauchsanweisung:
var hash = CreateHash();
HashPut(hash, "foo", "bar"); // "bar" unter dem Key "foo" speichern
HashPut(hash, FindObject(CLNK), 42); // Wert 42 mit einem Objekt als Key speichern
var bar = HashGet(hash, "foo"); // Wert mit dem Key "foo" wieder auslesen

 */

#strict 3

/** Create an empty hash. */
global func CreateHash() {
	return [nil, [[]]];
}

/** Insert a value into a hash.
 * \param[in,out] hash The hash to modify
 * \param[in] key The key of the element
 * \param[in] val The value of the element
 * \return An iterator pointing in front of the inserted element
 */
global func HashPut(&hash, key, val) {
	_Hash_Resize(hash);
	var hashval = _Hash_HashVal(key);
	var bidx = _Hash_UMod(hashval, GetLength(hash[(_Hash_Bucket ?? 0)]));
	if(!Inside(bidx, nil, (GetLength(hash[(_Hash_Bucket ?? 0)]) ?? 0) - 1))
		Log("bidx out of bounds: %d (of %d)", bidx, (GetLength(hash[(_Hash_Bucket ?? 0)]) ?? 0) - 1);
	for(var i = nil; (i ?? 0) < (GetLength(hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)]) ?? 0); S42MigrationStep(i, 1)) {
		if(S42MigrationEqual(key, hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)][(_HashNode_Key ?? 0)])) {
			hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)][(_HashNode_Val ?? 0)] = val;
			return [hash, bidx, i];
		}
	}
	var ni = GetLength(hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)]);
	hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(ni ?? 0)] =
		[key, val, hashval];
	S42MigrationStep(hash[(_Hash_Size ?? 0)], 1);
	
	return [hash, bidx, ni];
}

/** Find a value in a hash.
 * \param[in] hash The haystack
 * \param[in] key The needle
 * \return An iterator pointing in front of the inserted element. If no element with the
 * given \p key exists, an iterator pointing behind the last element is returned.
 * \sa HashIterHasNext
 */
global func HashFind(&hash, key) {
	var hashval = _Hash_HashVal(key);
	var bidx = _Hash_UMod(hashval, GetLength(hash[(_Hash_Bucket ?? 0)]));
	for(var i = nil; (i ?? 0) < (GetLength(hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)]) ?? 0); S42MigrationStep(i, 1)) {
		if(S42MigrationEqual(key, hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)][(_HashNode_Key ?? 0)])) {
			return [hash, bidx, i];
		}
	}
	var iter = HashIter(hash);
	return HashIterToBack(iter);
}

/** Checks for the existence of an element with a given key.
 * \param[in] hash The hash to search
 * \param[in] key The key to find
 * \return \c true if an element with the key \c key exists, \c false otherwise.
 */
global func HashContains(&hash, key) {
	var iter = HashFind(hash, key);
	return HashIterHasNext(iter);
}

/** Get a value from a hash.
 * \param[in] hash The hash to search in
 * \param[in] key The key of the wanted element
 * \param[in] returnSuccess Whether to indicate success with the return value
 * \return If \p returnSuccess is \c false, returns the value of the element with the given
 * \p key, or 0 if no element is found. If \p returnSuccess is \c true, returns an array
 * which has the value as first element. The second element indicates whether a match was
 * found.
 */
global func &HashGet(&hash, key, bool returnSuccess) {
	returnSuccess ??= false;
	var hashval = _Hash_HashVal(key);
	var bidx = _Hash_UMod(hashval, GetLength(hash[(_Hash_Bucket ?? 0)]));
	for(var i = nil; (i ?? 0) < (GetLength(hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)]) ?? 0); S42MigrationStep(i, 1)) {
		if(S42MigrationEqual(key, hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)][(_HashNode_Key ?? 0)])) {
			if(returnSuccess)
				return [hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)][(_HashNode_Val ?? 0)], true];
			else
				return hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)][(_HashNode_Val ?? 0)];
		}
	}
	if(returnSuccess)
		return [nil, nil];
	else
		return nil;
}

/** Remove an element from a hash.
 * \param[in,out] hash The hash to modify
 * \param[in] key The key of the element to be removed
 * \return \c true if an element was removed, \c false otherwise.
 */
global func HashErase(&hash, key) {
	var hashval = _Hash_HashVal(key);
	var bidx = _Hash_UMod(hashval, GetLength(hash[(_Hash_Bucket ?? 0)]));
	var found = nil;
	for(var i = nil; (i ?? 0) < (GetLength(hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)]) ?? 0); S42MigrationStep(i, 1)) {
		if(found) {
			hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0) - 1] = hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)];
		} else if(S42MigrationEqual(key, hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)][(i ?? 0)][(_HashNode_Key ?? 0)])) {
			found = true;
		}
	}
	if(found) {
		SetLength(hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)],
		          (GetLength(hash[(_Hash_Bucket ?? 0)][(bidx ?? 0)]) ?? 0) - 1);
		S42MigrationStep(hash[(_Hash_Size ?? 0)], -1);
	}
	_Hash_Resize(hash);
	return found;
}

/** Returns the number of elements in the hash.
 */
global func HashSize(&hash) {
	return hash[(_Hash_Size ?? 0)];
}

/** Create an iterator on a hash.
 * \param[in] hash The hash to iterate over
 */
global func HashIter(&hash) {
	return [hash, nil, nil];
}

/** Checks whether the iterator can advance.
 * \param[in] iter The iterator in question
 * \return \c true if there is at least one more element, \c false if \p iter points at the
 * back of the corresponding hash.
 */
global func HashIterHasNext(&iter) {
	var ni = iter[(_HashIter_Node ?? 0)];
	for(var bi = iter[(_HashIter_Bucket ?? 0)];
	    (bi ?? 0) < (GetLength(iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)]) ?? 0);
	    S42MigrationStep(bi, 1))
	{
		if((ni ?? 0) < (GetLength(iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)][(bi ?? 0)]) ?? 0))
			return true;
		ni = nil;
	}
	return nil;
}

/** Advances the iterator.
 * \param[in,out] iter The iterator to advance
 * \return An array of [key, value] of the element traversed, or 0 if \p iter pointed at
 * the back of the hash.
 */
global func HashIterNext(&iter) {
	var ni = iter[(_HashIter_Node ?? 0)];
	var val;
	for(var bi = iter[(_HashIter_Bucket ?? 0)];
	    (bi ?? 0) < (GetLength(iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)]) ?? 0);
	    S42MigrationStep(bi, 1))
	{
		if((ni ?? 0) < (GetLength(iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)][(bi ?? 0)]) ?? 0)) {
			val = iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)][(bi ?? 0)][(ni ?? 0)];
			iter[(_HashIter_Bucket ?? 0)] = bi;
			iter[(_HashIter_Node ?? 0)] = (ni ?? 0) + 1;
			return val;
		}
		ni = nil;
	}
	return nil;
}

/** Checks whether the iterator can step back.
 * \param[in] iter The iterator in question
 * \return \c true if there is at least one element before the current one, \c false if \p
 * iter points at the beginning of the hash.
 */
global func HashIterHasPrev(&iter) {
	if(iter[(_HashIter_Node ?? 0)]) return true;
	for(var bi = (iter[(_HashIter_Bucket ?? 0)] ?? 0) - 1; (bi ?? 0) >= 0; S42MigrationStep(bi, -1)) {
		if(GetLength(iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)][(bi ?? 0)])) return true;
	}
	return nil;
}

/** Moves the iterator backwards.
 * \param[in,out] iter The iterator to move
 * \return An array of [key, value] of the element traversed, or 0 if \p iter pointed at
 * the beginning of the hash.
 */
global func HashIterPrev(&iter) {
	if(iter[(_HashIter_Node ?? 0)]) {
		return iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)][(iter[(_HashIter_Bucket ?? 0)] ?? 0)]
		       [S42MigrationStep(iter[(_HashIter_Node ?? 0)], -1)];
	}
	for(var bi = (iter[(_HashIter_Bucket ?? 0)] ?? 0) - 1; (bi ?? 0) >= 0; S42MigrationStep(bi, -1)) {
		iter[(_HashIter_Node ?? 0)] = GetLength(iter[(_HashIter_Hash ?? 0)]
		                                 [(_Hash_Bucket ?? 0)][(bi ?? 0)]);
		if((iter[(_HashIter_Node ?? 0)] ?? 0) > 0) {
			iter[(_HashIter_Bucket ?? 0)] = bi;
			return iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)][(iter[(_HashIter_Bucket ?? 0)] ?? 0)]
			       [S42MigrationStep(iter[(_HashIter_Node ?? 0)], -1)];
		}
	}
	iter[(_HashIter_Bucket ?? 0)] = iter[(_HashIter_Node ?? 0)] = nil;
	return nil;
}

/** Moves the iterator to the front of the hash.
 */
global func HashIterToFront(&iter) {
	iter[(_HashIter_Bucket ?? 0)] = iter[(_HashIter_Node ?? 0)] = nil;
	return iter;
}

/** Moves the iterator to the back of the hash.
 */
global func HashIterToBack(&iter) {
	iter[(_HashIter_Bucket ?? 0)] = (GetLength(iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)]) ?? 0) - 1;
	iter[(_HashIter_Node ?? 0)] = GetLength(iter[(_HashIter_Hash ?? 0)][(_Hash_Bucket ?? 0)]
	                                 [(iter[(_HashIter_Bucket ?? 0)] ?? 0)]);
	return iter;
}

/*----        Implementation detail follows.         ----*
 *---- Function presence and/or semantics may change ----*
 *----   at any time. Do NOT rely on these to last.  ----*
 *----             YOU HAVE BEEN WARNED.             ----*/

// Array indices
static const _Hash_Size = 0;
static const _Hash_Bucket = 1;
static const _HashNode_Key = 0;
static const _HashNode_Val = 1;
static const _HashNode_Hash = 2;
static const _HashIter_Hash = 0;
static const _HashIter_Bucket = 1;
static const _HashIter_Node = 2;

global func _Hash_DebugDump(&hash) {
	Log("Hash debug dump");
	Log("Entries: %d. Buckets: %d. Load factor: %d.", hash[(_Hash_Size ?? 0)],
	    GetLength(hash[(_Hash_Bucket ?? 0)]), _Hash_LoadFactor(hash));
	Log("--------------------");
	var iter = HashIter(hash);
	var v;
	for(;;) {
		v = HashIterNext(iter);
		if(!v) break;
		Log("[%d @%d,%d] %s: %v => %s: %v", v[2], iter[(_HashIter_Bucket ?? 0)],
		    (iter[(_HashIter_Node ?? 0)] ?? 0) - 1,
		    _Hash_DebugDump_Type(GetType(v[0])), v[0],
		    _Hash_DebugDump_Type(GetType(v[1])), v[1]);
	}
	Log("--------------------");
}

global func _Hash_DebugDump_Type(int t) {
	t ??= 0;
	if(S42MigrationEqual(t, C4V_String)) return "string";
	if(S42MigrationEqual(t, C4V_Int)) return "int";
	if(S42MigrationEqual(t, C4V_C4Object)) return "object";
	if(S42MigrationEqual(t, C4V_C4ID)) return "id";
	if(S42MigrationEqual(t, C4V_Array)) return "array";
	if(S42MigrationEqual(t, C4V_Bool)) return "bool";
	return "any";
}

global func _Hash_Resize(&hash) {
	var lf = _Hash_LoadFactor(hash);
	var buckets, bc = GetLength(hash[(_Hash_Bucket ?? 0)]);
	if((lf ?? 0) > 75) {
		// MOAR BUCKETS!
		S42MigrationAssign(bc, "*=", 2);
	} else if((lf ?? 0) < 25) {
		// Less buckets
		S42MigrationAssign(bc, "/=", 2); S42MigrationStep(bc, 1);
	}
	if(S42MigrationEqual(bc, GetLength(hash[(_Hash_Bucket ?? 0)])))
		return;
	
	buckets = CreateArray(bc);
	for(var i = nil; (i ?? 0) < (bc ?? 0); S42MigrationStep(i, 1)) {
		buckets[(i ?? 0)] = [];
	}
	// Move each node to new bucket
	for(var b in hash[(_Hash_Bucket ?? 0)]) {
		for(var n in b) {
			buckets[(_Hash_UMod(n[(_HashNode_Hash ?? 0)], bc) ?? 0)]
			       [(GetLength(buckets[(_Hash_UMod(n[(_HashNode_Hash ?? 0)],bc) ?? 0)]) ?? 0)] = n;
		}
	}
	hash[(_Hash_Bucket ?? 0)] = buckets;
}

global func _Hash_LoadFactor(&hash) {
	return (hash[(_Hash_Size ?? 0)] ?? 0)*100/(GetLength(hash[(_Hash_Bucket ?? 0)]) ?? 0);
}

global func _Hash_HashVal(val) {
	var h = nil;
	if(!val) {
		return nil;
	} else if(S42MigrationEqual(GetType(val), C4V_String)) {
		// sdbm hash function
		for(var i = nil, c; c = GetChar(val, S42MigrationStep(i, 1, true));) {
			h = (c ?? 0) + ((h ?? 0) << 16) + ((h ?? 0) << 6) - (h ?? 0);
		}
		return h;
	} else if(S42MigrationEqual(GetType(val), C4V_Int) || S42MigrationEqual(GetType(val), C4V_Bool)) {
		// cf. Knuth, sec. 6.4
		return (val ?? 0) * 0x9e3779b1;
	} else if(S42MigrationEqual(GetType(val), C4V_C4Object)) {
		return _Hash_HashVal(val->ObjectNumber());
	} else if(S42MigrationEqual(GetType(val), C4V_Array)) {
		for(var v in val) {
			h = (_Hash_HashVal(v) ?? 0) + ((h ?? 0) << 16) + ((h ?? 0) << 6) - (h ?? 0);
		}
		return h;
	} else if(S42MigrationEqual(GetType(val), C4V_C4ID)) {
		return _Hash_HashVal(CastInt(val));
	} else {
		FatalError("Type not hashable");
	}
}

global func _Hash_UMod(int a, int b) {
	a ??= 0;
	b ??= 0;
	if((a ?? 0) < 0) S42MigrationAssign(a, "+=", ((Abs((a ?? 0)/(b ?? 0)) ?? 0) + 1) * (b ?? 0));
	return (a ?? 0) % (b ?? 0);
}
