#include "ft_nm.h"

char **name_tab;
t_all *g_all = NULL;

void	ft_lstadd_back(t_all **alst, t_all *new)
{
	t_all	*lst;

	lst = *alst;
	if (*alst == NULL)
		*alst = new;
	else
	{
		while (lst->next)
			lst = lst->next;
		lst->next = new;
	}
}

t_all	*fill_all(uint64_t address, char* name, char symbole, int type)
{
	t_all	*lst = malloc(sizeof(t_all));

	lst->address = address;
    lst->name = name;
    lst->symbole = symbole;
    lst->type = type;
    lst->type = 0;
    lst->next = NULL;
}

int check_arg(char *binary_path, int bits)
{
    int i = 0;
    FILE *fp = fopen(binary_path, "rb");
    if (!fp) {
        fprintf(stderr, "Impossible d'ouvrir le fichier binaire %s.\n", binary_path);
		return 1;
    }
    Elf64_Ehdr elf_header;
    
    if (fread(&elf_header, sizeof(elf_header), 1, fp) != 1) {
        //fprintf(stderr, "Erreur de lecture de l'en-tête ELF.\n");
        bits = 1;
    }
    if (elf_header.e_ident[0] != 0x7f ||
        elf_header.e_ident[1] != 'E' ||
        elf_header.e_ident[2] != 'L' ||
        elf_header.e_ident[3] != 'F')
    {
        //printf("not elf file \n");
        return (1);
    } 
    while(binary_path[i] != '\0')
    {
        i++;
    }
    i--;
    //printf("str = %c\n", binary_path[i]);
    if (ft_strlen(binary_path) > 3 && bits == 1)
    {
        if (binary_path[i] == 'o'){}
        else{
            printf("Invalid format \n");
            return 1;
        }
        if ((binary_path[i - 1] == 's' && binary_path[i - 2] == '.') || (binary_path[i - 1] == '.')){}
        else{
            printf("Invalid format2 \n");
            return 1;
        }
    }
    else
    {
        if (elf_header.e_ident[EI_CLASS] == ELFCLASS32)
            bits = 32;
        else if (elf_header.e_ident[EI_CLASS] == ELFCLASS64) 
            bits = 64;
        else {
            fclose(fp);
            return 1;
        }
    }
    return 0;
}

void    affichage(char **tab)
{
    int i = 0;

    while (tab[i] != NULL)
    {
        if (ft_strlen(tab[i]) > 0)
            printf("%d = %s\n", i, tab[i]);
        i++;
    }
    return ;
}

void    print_list()
{
    while (g_all->next)
    {
        printf("---------------------\n");
        printf("|g_all->addr = %016lx\n", g_all->address);
        printf("|g_all->name = %s\n", g_all->name);
        printf("|g_all->type = %d\n", g_all->type);
        printf("|g_all->symbole = %c\n", g_all->symbole);
        printf("---------------------\n");
        g_all = g_all->next;
    }
    return;
}
char    *all_in_min(char *str)
{
    // if (ft_strlen(str) == 0)
    //     return NULL;
    int i = 0;
    int lengh = ft_strlen(str);
    char *tab = malloc(sizeof(char) * (ft_strlen(str) + 1));

    while (str[i] != '\0') {
        if (str[i] >= 'A' && str[i] <= 'Z')
            tab[i] = str[i] + 32;
        else
            tab[i] = str[i];
        i++;
    }
    tab[i] = '\0';
    return tab;
}

int	ft_strlen_without_tiret(char *s)
{
	int	i;

	int count = 0;
    i = 0;
	if (s == NULL)
		return (0);
	while (s[i] != '\0')
	{
        if (s[i] != '_')
            count++;
        i++;
	}
	return (count);
}

int is_smaller(char *tmp, char* tab_str)
{
    char* str_min = all_in_min(tab_str);
    char *tmp_min = all_in_min(tmp);
    int try = 0;
    //printf("str = %s et tmp = %s\n", str_min, tab_str);
    int i = 0;
    int j = 0;
    while(tmp_min[i] == '_')
        i++;
    while(str_min[j] == '_')
        j++;
    if (i > j)
        try == 1;
    if (i < j)
        try == 2;
    //printf("i = %d et j = %d\n\n", i, j);
    while(tmp_min[i] && str_min[j])
    {
        if (tmp_min[i] == '_' && str_min[j] != '_' && i == j)
        {
            free(tmp_min);
            free(str_min);
            return 1;
        }
        if (tmp_min[i] != '_' && str_min[j] == '_' && i == j)
        {
            free(tmp_min);
            free(str_min);
            return 0;
        }
        if (tmp_min[i] < str_min[j]){
            free(str_min);
            free(tmp_min);
            return 0;
        }
        if (tmp_min[i] > str_min[j]){
            free(tmp_min);
            free(str_min);
            return 1;
        }
        j++;
        i++;
    }
    if (ft_strlen_without_tiret(tmp_min) == ft_strlen_without_tiret(str_min))
    {
        if (tmp_min[0] == '_' && str_min[0] != '_')
        {
            free(tmp_min);
            free(str_min);
            return 0;
        }
    
        if (tmp_min[0] != '_' && str_min[0] == '_')
        {
            free(tmp_min);
            free(str_min);
            return 1;
        }
    
    }

    if (ft_strlen(str_min) < ft_strlen(tmp_min))
    {
        free(tmp_min);
        free(str_min);
        return 1;
    }
    free(tmp_min);
    free(str_min);
    return 0;
}

int size_tab()
{
    int size_tab = 0;

    while (name_tab[size_tab])
    {
        size_tab++;
    }
    return size_tab;
}

void    print_final(char **tri_tab)
{
    t_all *tmp = g_all;
    int i = 0;
    while(i< size_tab())
    {
        t_all *tmp = g_all;
        while (tmp)
        {
            if ((ft_strlen(tri_tab[i]) == ft_strlen(tmp->name)) && ft_strncmp(tri_tab[i], tmp->name, ft_strlen(tri_tab[i])) == 0 && tmp->write != 1)
            {
                if (tmp->address == 0)
                    printf("%16c %c %s\n", ' ', tmp->symbole, tmp->name);
                else
                    printf("%016lx %c %s\n", tmp->address, tmp->symbole, tmp->name);
                tmp->write = 1;
            }
            tmp = tmp->next;
        }
        i++;
    }
    return;
}

void free_all()
{
	t_all	*temp;
	while(g_all->next)
	{
		temp = g_all;
		g_all = g_all->next;
		free(temp);
	}
	free(g_all);
    g_all = NULL;
	return ;
}

void tri()
{
    //printf("ok\n");
    char *tmp = NULL;
    char **tri_tab = (char**)malloc(sizeof(char*) * (size_tab() + 1));
    int i = 0;
    int letter = 0;
    tmp = name_tab[0];
    int to_delete = 0;
    int to_fill = 0;
    while (i < size_tab())
    {
        //printf("ok = %d\n", i);
        while(letter < size_tab())
        {
            if (name_tab[letter] != ""){
                if (is_smaller(tmp, name_tab[letter]) == 1 ){
                    tmp = name_tab[letter];
                    to_delete = letter;
                }
            }
            letter++;
        }
        //printf("tmp = %s et tab = %s\n", tmp, name_tab[to_delete]);
        tri_tab[i] = tmp;
        i++;
        letter = 0;
        name_tab[to_delete] = "";
        for (size_t j = 0; j < size_tab(); j++)
        {
            if (name_tab[j] != "")
            {
                tmp = name_tab[j];
                to_delete = j;
                break;
            }
            to_delete = 0;
            
        }
        
        
        
    }
    //printf("oktgbg\n");
    tri_tab[i] = NULL;
    //affichage(tri_tab);
    print_final(tri_tab);
    free(tri_tab);
    free_all();
    return ;
}

void    free_tab()
{
    // int i = 0;

    // while(name_tab[i] != NULL)
    // {
    //     free(name_tab[i]);
    //     i++;
    // }
    free(name_tab);
    return ;
}

int main(int argc, char **argv)
{
    
    if (argc < 2){
        fprintf(stderr, "Usage: %s <binary> ... \n", argv[0]);
        return 1;
    }
    int nb_arg = 1;
    
    while (nb_arg < argc)
    {
        int count = 0;
        if (check_arg(argv[nb_arg], 0) == 1){
            printf("ft_nm: %s: file format not recognized\n", argv[nb_arg]);
        }
        else{
            int fd = open(argv[nb_arg], O_RDONLY);
            if (fd < 0) {
                printf("Impossible d'ouvrir le fichier %s\n", argv[nb_arg]);
                return 1;
            }
            void *mapped = mmap(NULL, 100000000000, PROT_READ, MAP_PRIVATE, fd, 0);
            if (mapped == MAP_FAILED) {
                printf("Erreur lors du mappage du fichier binaire en mémoire\n");
                close(fd);
                return 1;
            }

            // Obtenir l'en-tête du fichier binaire
            Elf64_Ehdr *ehdr = (Elf64_Ehdr *) mapped;

            Elf64_Shdr *shdr = (Elf64_Shdr *) (mapped + ehdr->e_shoff);
            Elf64_Shdr *shstrtab = &shdr[ehdr->e_shstrndx];
            char *strtab = (char *)(mapped + shstrtab->sh_offset);
            Elf64_Sym *symtab = NULL;
            char *symtab_str = NULL;
            int i, symtab_count;
            if (argc > 2)
            {
                printf("\n");
                printf("%s:\n", argv[nb_arg]);
            }
            for (i = 0; i < ehdr->e_shnum; i++) {
                if (shdr[i].sh_type == SHT_SYMTAB) {
                    symtab = (Elf64_Sym *)(mapped + shdr[i].sh_offset);
                    symtab_count = shdr[i].sh_size / sizeof(Elf64_Sym);
                    symtab_str = (char *)(mapped + shdr[shdr[i].sh_link].sh_offset);
                    char *strtab = (char *) (mapped + shstrtab->sh_offset);
                    for (int j = 0; j < symtab_count; j++) {
                        //count = j;
                        if (j == 0)
                            name_tab = (char**)malloc(sizeof(char*) * (symtab_count ));
                        unsigned char symbol_type_and_binding = symtab[j].st_info;
                        unsigned char symbol_type = symbol_type_and_binding & 0x0F; // les 4 bits de poids faible contiennent le type
                        unsigned char symbol_binding = symbol_type_and_binding >> 4;
                        print(j, symbol_type, symbol_binding, symtab_str, symtab);
                        //ft_lstadd_back(&g_all, fill_all(symtab[j].st_value, symtab_str + symtab[j].st_name, symbol_type, 0));
                        if (ft_strlen(symtab_str + symtab[j].st_name) > 0 && symbol_type != 4){
                            name_tab[count] = symtab_str + symtab[j].st_name;
                            count++;
                        }
                    }
                    
                    break;
                }
                
            }
            //print_list();
            name_tab[count] = NULL;
            //printf("\n");
            //affichage(name_tab);
            tri();
            free_tab();
            munmap(mapped, 100000000000);
            close(fd);
        }
        
        nb_arg++;
    }
    // name_tab[count] = NULL;
    // affichage();
    // tri();
    return 0;
}
