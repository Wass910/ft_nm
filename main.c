#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

const char *symbol_types[] = {
    "U", // STT_NOTYPE
    "A", // STT_OBJECT
    "F", // STT_FUNC
    "S", // STT_SECTION
    "T", // STT_FILE
    "C", // STT_COMMON
    "T", // STT_TLS
    NULL,
    NULL,
    "r", // STT_RELC
    "R", // STT_SRELC
    "i", // STT_GNU_IFUNC
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "v", // STT_LOOS
    "V", // STT_HIOS
    "l", // STT_LOPROC
    "P", // STT_HIPROC
};

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
        printf("not elf file \n");
        exit(1);
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
        printf("e_type = %u\n", elf_header.e_type);
        printf("e_machine = %d\n", elf_header.e_machine);
        printf("e_version = %d\n", elf_header.e_version);
        printf("e_ehsize = %d\n", elf_header.e_ehsize);
        
        if (elf_header.e_ident[EI_CLASS] == ELFCLASS32)
            bits = 32;
        else if (elf_header.e_ident[EI_CLASS] == ELFCLASS64) 
            bits = 64;
        else {
            fprintf(stderr, "Binaire incompatible avec la machine actuelle.\n");
            fclose(fp);
            return 1;
        }
    }
    return 0;
}

const char *get_symbol_type(unsigned char st_info) {
    int type = ELF64_ST_TYPE(st_info);
    if (type < sizeof(symbol_types) / sizeof(char *) && symbol_types[type] != NULL) {
        return symbol_types[type];
    } else {
        return "?";
    }
}

int is_undefined_symbol(Elf64_Sym *sym) {
    unsigned char st_info = sym->st_info;
    int type = ELF64_ST_TYPE(st_info);
    int bind = ELF64_ST_BIND(st_info);
    return (type == STT_NOTYPE && (bind == STB_GLOBAL || bind == STB_WEAK));
}

int main(int argc, char **argv)
{
    if (argc == 2){
        if (check_arg(argv[1], 0) == 1)
            return 1;
    }
    else{
        fprintf(stderr, "Usage: %s <binary>\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
        return 1;
    }

    // Obtenir la taille du fichier binaire
    off_t fsize = lseek(fd, 0, SEEK_END);
    if (fsize < 0) {
        printf("Erreur lors de l'obtention de la taille du fichier binaire\n");
        close(fd);
        return 1;
    }

    void *mapped = mmap(NULL, fsize, PROT_READ, MAP_PRIVATE, fd, 0);
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

    for (i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type == SHT_SYMTAB) {
            symtab = (Elf64_Sym *)(mapped + shdr[i].sh_offset);
            symtab_count = shdr[i].sh_size / sizeof(Elf64_Sym);
            symtab_str = (char *)(mapped + shdr[shdr[i].sh_link].sh_offset);
            unsigned char symbol_type_and_binding = symtab[i].st_info;
            unsigned char symbol_type = symbol_type_and_binding & 0x0F; // les 4 bits de poids faible contiennent le type
            unsigned char symbol_binding = symbol_type_and_binding >> 4;
            char *strtab = (char *) (mapped + shstrtab->sh_offset);
            for (int j = 0; j < symtab_count; j++) {
                unsigned char symbol_type_and_binding = symtab[j].st_info;
                unsigned char symbol_type = symbol_type_and_binding & 0x0F; // les 4 bits de poids faible contiennent le type
                unsigned char symbol_binding = symbol_type_and_binding >> 4;
                printf("Nom de symbole : %s\n", symtab_str + symtab[j].st_name);
                printf("Type de symbole : %u\n", symbol_type);
                printf("Adresse de symbole : %lx\n", symtab[j].st_value);
                printf("%s | %s\n", is_undefined_symbol(&symtab[j]) ? "U" : " ", strtab + symtab[j].st_name);
                printf("\n");
            }

            break;
        }
    }

    // Unmap le fichier binaire en mémoire
    munmap(mapped, fsize);
    close(fd);

    printf("valide format \n");
    return 0;
}
