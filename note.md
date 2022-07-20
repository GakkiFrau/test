**step**

1. 引脚配置模式，按下按键读取引脚状态（外部中断）
2. 



结构体：包含按键引脚，状态，函数指针，按键事件处理的函数指针数组





定义一个结构体Button

```c
#define NUM_OF_EVENT	8			

typedef void (*button_callback)(void*); // 不是很理解这里为什么用`void *`,函数实现时却没有用到

typedef struct Button
{
    uint8_t ActiveLevel		: 1;
    uint8_t ButtonLevel		: 1;
    uint8_t ButtonPin		   ;
    // press release single_press double_press long_press ...
    uint8_t Event			: 4;
    uint8_t State			: 3;
    // throuth function pointer get button level state 
    uint8_t (*get_button_level)(void);	  
    // define function pointer array for press event. 
    button_callback Cb[NUM_OF_EVENT];
    // list poiinter 
    struct Button* Next;  
}Button_s;
```





通过函数返回引脚状态

```c
// 结构体中定义了函数指针，外部实现函数然后作为参数让结构体中的函数指针指向函数
uint8_t read_button_pin(void)
{
    return KEY;
}

void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t actLevel)
{
    ...
    handle->get_button_level = pin_level;
    handle->ButtonLevel = handle->get_button_level();
    ...
}

```

通过函数处理按键事件

```c

// button_attach()

{
    
}
```







```c
/* handle->next指向head_handle 头指针地址， 然后头指针指向handle，即handle的值给到头指针head_handle, 所以头指针head_handle数据域和指针域内容与headle一致，并且指针域指向head_handle头指针地址 */
// button start
int button_start(struct Button* handle)
{
    struct Button* target = head_handle;
    while(target){
        if(target == handle) return -1; // already exist
        target = target->Next;
    }
    handle->Next = head_handle;
    head_handle = handle;
    return 0
}

void button_ticks()
{
    struct Button* target;
    for(target = head_handle; target; target = target->Next){
        button_handler(target);
    }
}
```







```c

/*
	1. 读取按键引脚电平状态
	2. 按键反弹处理
	3. 按键按下工作处理
		- 状态 0：
					if(按键按下){
						
						clear ticks
						jump 状态 1
					}
					else{	// 按键未按下
					
					}
		- 状态 1：
					if(按键释放){
						
						clear ticks
						jump 状态 2
					}
					else if(判断是否满足长按条件){	// 按键仍按下，判断是否长按
						
						Jump 状态 5
					}
		 - 状态 2： 
		 			if(按键按下){  	// 表示再次按下
		 				
		 				repeat++;		// 按下次数累加
		 				clear ticks
		 				jump 状态 3
		 			}
		 			else if(判断ticks是否大于短击时间){
		 				if(按下次数为1){
		 					单击
		 				}
		 				else if(按下次数为2){
		 					双击
		 				}
		 				
		 				clear 状态	// 判断出了是单击双击，所以清除所有状态
		 			}
		  - 状态 3：
		  			if(按键释放){
		  				if(ticks小于短击时间){
		  					clear ticks
		  					jump 状态 2		// repeat press
		  				}
		  				else{
		  					clear 状态
		  				}
		  			}
		   - 状态 5：
		   			if(按键按下){	// 长按之后还继续按下
		   				//长按保持
		   			}
		   			else{	// 释放
		   				clear 状态
		   			}

*/




```





<p style="color:blue ">blue</p>

<font color="red">state</font>









共用体

union

```C
typedef union {
    uint8_t data[FlASH_DATA_SAVE_LEN];
    struct {
        uint8_t perMacAddr[6];		//从机mac地址
        uint8_t sysData[FLASH_SYS_DATA_LEN];
    }Flash_Save_t;
}Flash_Save_u;

Flash_Save_u flash_data_save;

hal_flash_read(USER_DTAA_FLASH_ADDR, &flash_data_save.data[0], FLASH_DATA_SAVE_LEN);  // 读取flash保存到flash_data_save.data

if (flash_data_save.Flash_Save_t.sysData[0] == 0x01){	//OTA置位，OTA广播
    multiBle_adv_name_set(0, 1); /* 普通广播名称 */
}
else {
    multiBle_adv_name_set(0, 0); /* 普通广播名称 */
}
```



```c

#define B_ADDR_LEN			6
typedef struct mCentralDev
{
    uint8_t addrType;
    uint8_t addr[B_ADDR_LEN];
    struct mCentralDev* next;
}GAPMultiRoleCentralDev_t;

GAPMultiRoleCentralDev_t* g_mlinkingList = NULL; // devices

void multiAddSlaveConnList(uint8_t addrType, uint8_t * addr) 
{
    GAPMultiRoleCentralDev_t node;
    osal_memset(&node, 0, sizeof(GAPMultiRoleCentralDev_t));
    node.next = NULL;
    node.addrType = addrType;
    osal_memcpy(node,addr, addr, B_ADDR_LEN);
    multiListInSertTail(MULTI_CONFIG_MASTER_LINKING_MODE, (void **)&g_mlinkingList, &node);
    
}

void multiDelCurrentConnNode(void)
{
    if (g_mlinkingList) {
        GAPMultiRoleCentralDev_t* node = g_mlinkingList->next;
        osal_memset(g_mlinkingList, 0, sizeof(GAPMultiRoleCentralDev_t));
        osal_men_free(g_mlinkingList);
        
        if (node == NULL) {
            g_mlinkingList = NULL;
        }
        else {
            g_mlinkingList = node;
        }
    }
    
}

GAPMultiRoleCentralDev_t * multiGetSlaveConnList(void)
{
    return g_mlinkingList;
}
```







```c
/** 枚举 定义，作为函数参数进行条件执行 **/
typedef enum
{
    ...
    MULTI_CONFIG_MASTER_LINKING_MODE,
    ...
    
}GAPMultiListMode_t;

/** scanner 结构体链表 **/
typedef struct multiScan
{
    ...
    struct multiScan* next;
}GAPMultiRolScanner_t;

/** multiListCreat **/
static void* multiListCreate(GAPMultiListMode_t mode)
{
    void* node = NULL;
    switch(mode){
            ...
                case MULTI_CONFIG_MASTER_LINKING_MODE:
            	{
            		GAPMultiRoleCentralDev_t* tnode = (GAPMultiRoleCentralDev_t*)osal_mem_alloc(sizeof(GAPMultiRoleCentralDev_t));
            		if(tnode) {
                        osal_memset(tnode, 0, sizeof(sizeof(GAPMultiRoleCentralDev_t)));
                        tnode->next = NULL;
                    }
            		node = (GAPMultiRoleCentralDev_t*)tnode;
                 }
            	 break;
            ...
    }
    return node;
}

/** multiListFindTail **/
static void* multiListFindTail(GAPMultiListMode_t mode, void** ppnode)
{
    void* node = NULL;
    
    switch(mode)
    {
            ...
            case MULTI_CONFIG_MASTER_LINKING_MODE:
            {
                GAPMultiRoleCentralDev_t* entry = *(GAPMultiRoleCentralDev_t**)ppnode;
                
                while(entry) {
                    if(entry->next == NULL)
                        break;
                    entry = entry->next;
                }
                
                node = entry;
            }
            break;
                
            ...
    }
    
}

/** multiListInsertTail **/
uint8_t multiListInsertTail(GAPMultiListMode_t mode, void** ppnode, void* vnode)
{
    volatile uint8_t ret =FALSE;

    switch(mode) {
            ...
                
            case MULTI_CONFIG_MASTER_LINKING_MODE:
            {
               GAPMultiRoleCentralDev_t** curr = (GAPMultiRoleCentralDev_t**)ppnode;
                GAPMultiRoleCentralDev_t* new_node = multiListCreate(mode);
                
                if (new_node) {
                    osal_memcpy(new_node, (GAPMultiRoleCentralDev_t*)vnode, sizeof(GAPMultiRoleCentralDev_t));
                    GAPMultiRoleCentralDev_t* entry = multiListFindTail(mode, (void**)ppnode);
                    if (entry == NULL){
                        *curr = new_node;
                    }
                    else {
                        entry->next = new_node;
                    }
                    
                    ret = TRUE;
                }
            }
            break;

            ...
                    
    }
}
```





```c
/** multiListDelNode **/
static void multiListDelNode(GAPMultiListMode_t mode, void** ppnode, void* pvalue)
{
    switch (mode) {
            ...
            case MULTI_CONFIG_SLAVE_DEV_MODE:
            {
                for(GAPMultiRoleCentralDev_t** curr = (GAPMultiRoleCentralDev_t**)ppnode; *curr)
                {
                    GAPMultiRoleCentralDev_t* entry = *curr;
                    
                    if(osal_memcmp(entry->addr, (uint8*)pvalue, B_ADDR_LEN) == TRUE) {
                        *curr = entry->next;
                        osal_mem_free(entry);
                        break;
                    }
                    else
                    {
                        curr = &entry->next;
                    }
                }
            }
            break;
            ...
    }
}

```







```c
static void* multiList_inside(GAPMultiListMode_t mode, void** ppnode, void* pvalue)
{
    void* node = NULL;
    
    switch(mode) {
            ...
            case MULTI_CONFIG_MASTER_LINKING_MODE:
            {
                GAPMultiRoleCentralDev_t* entry = *(GAPMuttiRoleCentralDev_t**)ppnode;
                
                while(entry) {
                    if(osal_memcmp(entry->addr, (uint8_t *)pvalue, B_ADDR_LEN) == TRUE)
                        break;
                    
                    entry = entry->next;
                }
                node = (GAPMultiRoleCentralDev_t*)entry;
            }
            break;
            ...
    }
}
```















































