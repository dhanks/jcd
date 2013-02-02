#if !(defined JCD_LIST)
	#define JCD_LIST

#define LIST_ADD(a, b, c) list_add(a, b, c, __FILE__, __LINE__)
#define LIST_REMOVE(a, b) list_remove(a, b, __FILE__, __LINE__)

struct s_node
{
	char		file[MAX_STRING+1];
	int		line;
	void		*data;
	unsigned long	length;
	unsigned long	inuse;
	struct s_node	*next, *prev;
	struct s_list	*parent;
};

struct s_list
{
	char		name[MAX_LINE+1];
	unsigned int	nodes;
	struct
	{
		unsigned long	data;
		unsigned long	structure;
	} memory;
	struct s_node	*head;
	struct s_list	*next, *prev;
};

typedef struct s_node node;
typedef struct s_list list;

#endif
