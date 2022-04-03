struct buf_pool_t{

	ut_list_base<buf_page_t, ut_list_node<buf_page_t> buf_page_t::*> buf_pool_t::free;
	ut_list_base<buf_page_t, ut_list_node<buf_page_t> buf_page_t::*> flush_list;
	ut_list_base<buf_page_t, ut_list_node<buf_page_t> buf_page_t::*> LRU;
	ut_list_base<buf_page_t, ut_list_node<buf_page_t> buf_page_t::*> unzip_LRU;
}
/*******************************************************************//**
The two-way list base node. The base node contains pointers to both ends
of the list and a count of nodes in the list (excluding the base node
from the count). We also store a pointer to the member field so that it
doesn't have to be specified when doing list operations.
@param Type the type of the list element
@param NodePtr field member pointer that points to the list node */
template <typename Type, typename NodePtr>
struct ut_list_base {

	unsigned long count;	/*!< count of nodes in list */
	Type*	start;			/*!< pointer to list start, NULL if empty */
	Type*	end;			/*!< pointer to list end, NULL if empty */
	NodePtr	node;			/*!< Pointer to member field that is used as a link node */
};

/*******************************************************************//**
The two way list node.
@param TYPE the list node type name */
template <typename Type>
struct ut_list_node {
	Type* prev; /*!< pointer to the previous node, NULL if start of list */
	Type* next; /*!< pointer to next node, NULL if end of list */
};