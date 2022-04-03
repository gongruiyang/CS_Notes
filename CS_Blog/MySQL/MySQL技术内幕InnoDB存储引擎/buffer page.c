/** The common buffer control block structure
for compressed and uncompressed frames */
class buf_page_t {
public:
	/** Page id. Protected by buf_pool mutex. */
	page_id_t	id;

	/** Page size. Protected by buf_pool mutex. */
	page_size_t	size;

	/** Count of how many fold this block is currently bufferfixed. */
	uint32_t	buf_fix_count;

	/** type of pending I/O operation; 
	also protected by buf_pool->mutex for writes only */
	buf_io_fix	io_fix;

	/** Block state. @see buf_page_in_file */
	buf_page_state	state;
};


/** Page identifier. */
class page_id_t {
private:
	/** Tablespace id. */
	uint32_t	m_space;
	/** Page number. */
	uint32_t	m_page_no;
	/** A fold value derived from m_space and m_page_no,
	used in hashing. */
	mutable unsigned long int	m_fold;
};

#define PAGE_SIZE_T_SIZE_BITS	17
/** Page size descriptor. 
Contains the physical and logical page size, 
as well as whether the page is compressed or not. */
class page_size_t {
public:
	/** Constructor from (physical, logical, is_compressed).
	@param[in]	physical	physical (on-disk/zipped) page size
	@param[in]	logical		logical (in-memory/unzipped) page size
	@param[in]	is_compressed	whether the page is compressed */
	page_size_t(ulint physical, ulint logical, bool is_compressed)
	{
		// 若未指定则 初始化为16kb
		if (physical == 0) {
			physical = (1 << 14);
		}
		if (logical == 0) {
			logical = (1 << 14);
		}

		m_physical = static_cast<unsigned>(physical);
		m_logical = static_cast<unsigned>(logical);
		m_is_compressed = static_cast<unsigned>(is_compressed);
	}
private:

	/* Disable implicit copying. */
	void operator=(const page_size_t&);

	/* For non compressed tablespaces, 
	physical page size is equal to the logical page size
	the data is stored in buf_page_t::frame
	(and is also always equal to univ_page_size (--innodb-page-size=)).

	For compressed tablespaces, 
	physical page size is the compressed page size as stored on disk and in buf_page_t::zip::data. 
	The logical page size is the uncompressed page size in memory - the size of buf_page_t::frame 
	(currently also always equal to univ_page_size
	(--innodb-page-size=)). */

	/** Physical page size. */
	unsigned	m_physical:PAGE_SIZE_T_SIZE_BITS;

	/** Logical page size. */
	unsigned	m_logical:PAGE_SIZE_T_SIZE_BITS;

	/** Flag designating whether the physical page is compressed, which is
	true IFF the whole tablespace where the page belongs is compressed. */
	unsigned	m_is_compressed:1;
};
