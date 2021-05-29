#include <stdio.h>
#include <stdlib.h>

#define CHARSIZE 1000
typedef struct _map
{
	unsigned size;
	char* addr;
	struct _map *next, *prior;
} map;
//维护一个全局变量头节点head
map *head;
//基地址
char* base;

void initialize()
{
//	initialize map
	head = malloc(sizeof(map));
//	initialize char
	head->addr = (char*)malloc(CHARSIZE * sizeof(char));
	head->size = CHARSIZE;
	head->next = head;
	head->prior = head;
	base = head->addr;
}

//释放系统函数malloc申请的内存
void free_all()
{
	while (head)
	{
		head = head->next;
		free(head->prior);
	}
	return;
}

void log_msg()
{
	//处理head为0的情况（直接分配1000）
	if (head->size == 0)
	{
		if (head->next != head)
		{
			map *p = head->next;
			head->size = p->size;
			head->addr = p->addr;
			head->next = p->next;
			p->next->prior = head;
		}
		else
		{
			printf("memory run out.\n");
			return;
		}
	}
	
	//删除size=0的节点
	map *h = head->next;
	while (h != head)
	{
		//h的size为0，则删除该节点
		if (!h->size)
		{
			h->prior->next = h->next;
			h->next->prior = h->prior;
		}
		h = h->next;
	}

	int cnt = 0;
	map *p = head;
	printf("available memory:\n");
	do {
		cnt += 1;
		printf("section%d: %ld -- %ld , size: %u bytes.\n", cnt, (long)p->addr-(long)base, (long)p->addr+p->size-1-(long)base, p->size);
		p = p->next;
	} while (p != head);
	return;
}

char* lmalloc(unsigned size)
{
	char* ret_addr = NULL;
//	size超过最大内存直接返回NULL
	if (size > CHARSIZE)
	{
		return NULL;
	}
//	循环寻找足够的空闲区
	map* current_p = head;
	do {
		if (current_p->size >= size)
		{
			ret_addr = current_p->addr;
			current_p->addr += size;
			current_p->size -= size;
			//如果当前空闲项size为0，调用库函数free()将其删除
/*			if (!current_p->size)
			{
				map *tmp = current_p->prior;
				tmp->next = current_p->next;
				current_p->next->prior = tmp;
				if (current_p != head)
				{
					free(current_p);
				}
			}*/
			return ret_addr;
		}
		else
		{
			current_p = current_p->next;
		}
	} while (current_p != head);
	return ret_addr;
}

void lfree(unsigned size, char* addr)
{
	map* current = head;
//	寻找包含addr的空闲区
	//地址越界
	if (addr < base || addr + size > base + CHARSIZE)
	{
		printf("address out of bound.\n");
		return;
	}
	
	//处理只有head的情况
	if (head->next == head)
	{
		//越界
		if ((head->addr<=addr&&head->addr+head->size>addr)||(head->addr<addr+size&&head->addr+head->size>=addr+size))
		{
			printf("address out of bound.\n");
			return;
		}
		else
		{
			map *new_p = malloc(sizeof(map));
			new_p->addr = addr;
			new_p->size = size;
			new_p->prior = head;
			head->next = new_p;
			new_p->next = head;
			head->prior = new_p;
		}
		printf("free %u bytes at address %ld.\n", size, (long)addr-(long)base);
		return;
	}
	
	while(current->addr < addr)
	{
		current = current->next;
	}
	map *prev = current->prior;
	//左边界与current前面的空闲区重叠
	if (addr < prev->addr + prev->size)
	{
		printf("min address %ld out of bound.\n", (long)addr-(long)base);
		return;
	}
	//右边界与current重叠
	if (addr + size > current->addr)
	{
		printf("max address %ld out of bound.\n", (long)addr+size-(long)base);
		return;
	}
	//处于非空闲区
	//与前空闲区相邻
	int cnt = 0;
	if (addr == prev->addr + prev->size)
	{
		prev->size += size;
		cnt += 1;
	}
	//与当前空闲区相邻
	if (addr + size == current->addr)
	{
		current->addr = addr;
		current->size += size;
		cnt += 1;
	}
	//以上两个if均执行，即都相邻，删除current
	if (cnt == 2)
	{
		prev->next = current->next;
		current->next->prior = prev;
		prev->size += current->size - size;
		if (head == current)
		{
			head = prev;
		}
		free(current);
	}
	//以上两个if均不执行，即不相邻，创建新节点插入current前面
	if (cnt == 0)
	{
		map *new_p = malloc(sizeof(map));
		new_p->addr = addr;
		new_p->size = size;
		new_p->prior = prev;
		prev->next = new_p;
		new_p->next = current;
		current->prior = new_p;
	}
	printf("free %u bytes at address %ld.\n", size, (long)addr-(long)base);
	return;
}

int main()
{
//	initialize head
	initialize();
	
	char command;
	char c;
	unsigned size;
	long addr;
	printf("$command:~");
	command = getchar();
	while (command != 'q')
	{
		c = getchar();
		scanf("%u", &size);
		if (command == 'm') 
		{
			addr = lmalloc(size);
			if (addr)
			{
				printf("alloc %u bytes at address %ld.\n", size, (long)addr-(long)base);
			}
			else
			{
				printf("memory not enough.\n");
			}
			log_msg();
		}
		else
		{
			c = getchar();
			scanf("%ld", &addr);
			lfree(size, addr+(long)base);
			log_msg();
		}
		c = getchar();	//read '\n' from the last command
		printf("$command:~");
		command = getchar();
	}
	printf("quit.\n");
	free_all();
	return 0;
}
