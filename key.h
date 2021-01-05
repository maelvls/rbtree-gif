void *key_create(int value);
void key_delete(void *key);
int key_cmp(const void *key1, const void *key2);
int key_equal(const void *key1, const void *key2);
int key_put(void *key); /* XXX DEBUG */
