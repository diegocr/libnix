
char *dcgettext(const char *domainname __attribute__((unused)),
	const char *msgid, int category __attribute__((unused)))
{
	return((char *) msgid );
}
