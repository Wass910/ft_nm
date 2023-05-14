#include "ft_nm.h"

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (6);
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}

int is_undefined_symbol(Elf64_Sym *sym) {
    unsigned char st_info = sym->st_info;
    int type = ELF64_ST_TYPE(st_info);
    int bind = ELF64_ST_BIND(st_info);
    return (type == STT_NOTYPE && (bind == STB_GLOBAL || bind == STB_WEAK));
}

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n <= 0)
		return (0);
	while (n > 1 && (*s1 != '\0' && *s2 != '\0') && *s1 == *s2)
	{
		s1++;
		s2++;
		n--;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}