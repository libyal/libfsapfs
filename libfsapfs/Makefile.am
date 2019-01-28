AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCTHREADS_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBCRYPTO_CPPFLAGS@ \
	@LIBHMAC_CPPFLAGS@ \
	@LIBCAES_CPPFLAGS@ \
	@ZLIB_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBFSAPFS_DLL_EXPORT@

lib_LTLIBRARIES = libfsapfs.la

libfsapfs_la_SOURCES = \
	fsapfs_btree.h \
	fsapfs_checkpoint_map.h \
	fsapfs_chunk_information_block.h \
	fsapfs_container_reaper.h \
	fsapfs_container_superblock.h \
	fsapfs_extent_reference_tree.h \
	fsapfs_file_system.h \
	fsapfs_fusion_middle_tree.h \
	fsapfs_key_bag.h \
	fsapfs_object.h \
	fsapfs_object_map.h \
	fsapfs_snapshot_metadata.h \
	fsapfs_space_manager.h \
	fsapfs_volume_superblock.h \
	libfsapfs.c \
	libfsapfs_btree_entry.c libfsapfs_btree_entry.h \
	libfsapfs_btree_footer.c libfsapfs_btree_footer.h \
	libfsapfs_btree_node.c libfsapfs_btree_node.h \
	libfsapfs_btree_node_header.c libfsapfs_btree_node_header.h \
	libfsapfs_buffer_data_handle.c libfsapfs_buffer_data_handle.h \
	libfsapfs_checkpoint_map.c libfsapfs_checkpoint_map.h \
	libfsapfs_checkpoint_map_entry.c libfsapfs_checkpoint_map_entry.h \
	libfsapfs_checksum.c libfsapfs_checksum.h \
	libfsapfs_chunk_information_block.c libfsapfs_chunk_information_block.h \
	libfsapfs_compressed_data_handle.c libfsapfs_compressed_data_handle.h \
	libfsapfs_compression.c libfsapfs_compression.h \
	libfsapfs_container.c libfsapfs_container.h \
	libfsapfs_container_data_handle.c libfsapfs_container_data_handle.h \
	libfsapfs_container_key_bag.c libfsapfs_container_key_bag.h \
	libfsapfs_container_reaper.c libfsapfs_container_reaper.h \
	libfsapfs_container_superblock.c libfsapfs_container_superblock.h \
	libfsapfs_data_block.c libfsapfs_data_block.h \
	libfsapfs_data_block_data_handle.c libfsapfs_data_block_data_handle.h \
	libfsapfs_data_stream.c libfsapfs_data_stream.h \
	libfsapfs_debug.c libfsapfs_debug.h \
	libfsapfs_definitions.h \
	libfsapfs_deflate.c libfsapfs_deflate.h \
	libfsapfs_directory_record.c libfsapfs_directory_record.h \
	libfsapfs_error.c libfsapfs_error.h \
	libfsapfs_encryption_context.c libfsapfs_encryption_context.h \
	libfsapfs_extended_attribute.c libfsapfs_extended_attribute.h \
	libfsapfs_extern.h \
	libfsapfs_extent_reference_tree.c libfsapfs_extent_reference_tree.h \
	libfsapfs_file_entry.c libfsapfs_file_entry.h \
	libfsapfs_file_extent.c libfsapfs_file_extent.h \
	libfsapfs_file_system_btree.c libfsapfs_file_system_btree.h \
	libfsapfs_file_system_data_handle.c libfsapfs_file_system_data_handle.h \
	libfsapfs_fusion_middle_tree.c libfsapfs_fusion_middle_tree.h \
	libfsapfs_inode.c libfsapfs_inode.h \
	libfsapfs_io_handle.c libfsapfs_io_handle.h \
	libfsapfs_key_bag_entry.c libfsapfs_key_bag_entry.h \
	libfsapfs_key_bag_header.c libfsapfs_key_bag_header.h \
	libfsapfs_key_encrypted_key.c libfsapfs_key_encrypted_key.h \
	libfsapfs_libbfio.h \
	libfsapfs_libcaes.h \
	libfsapfs_libcdata.h \
	libfsapfs_libcerror.h \
	libfsapfs_libclocale.h \
	libfsapfs_libcnotify.h \
	libfsapfs_libcthreads.h \
	libfsapfs_libfcache.h \
	libfsapfs_libfdata.h \
	libfsapfs_libfdatetime.h \
	libfsapfs_libfguid.h \
	libfsapfs_libhmac.h \
	libfsapfs_libuna.h \
	libfsapfs_lzvn.c libfsapfs_lzvn.h \
	libfsapfs_name.c libfsapfs_name.h \
	libfsapfs_name_hash.c libfsapfs_name_hash.h \
	libfsapfs_notify.c libfsapfs_notify.h \
	libfsapfs_object.c libfsapfs_object.h \
	libfsapfs_object_map.c libfsapfs_object_map.h \
	libfsapfs_object_map_btree.c libfsapfs_object_map_btree.h \
	libfsapfs_object_map_descriptor.c libfsapfs_object_map_descriptor.h \
	libfsapfs_password.c libfsapfs_password.h \
	libfsapfs_profiler.c libfsapfs_profiler.h \
	libfsapfs_snapshot_metadata.c libfsapfs_snapshot_metadata.h \
	libfsapfs_snapshot_metadata_tree.c libfsapfs_snapshot_metadata_tree.h \
	libfsapfs_space_manager.c libfsapfs_space_manager.h \
	libfsapfs_support.c libfsapfs_support.h \
	libfsapfs_types.h \
	libfsapfs_unused.h \
	libfsapfs_volume.c libfsapfs_volume.h \
	libfsapfs_volume_key_bag.c libfsapfs_volume_key_bag.h \
	libfsapfs_volume_superblock.c libfsapfs_volume_superblock.h

libfsapfs_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCTHREADS_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBHMAC_LIBADD@ \
	@LIBCAES_LIBADD@ \
	@LIBCRYPTO_LIBADD@ \
	@ZLIB_LIBADD@ \
	@LIBDL_LIBADD@ \
	@PTHREAD_LIBADD@

libfsapfs_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libfsapfs_definitions.h.in \
	libfsapfs.rc \
	libfsapfs.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libfsapfs_definitions.h
	/bin/rm -f libfsapfs.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libfsapfs ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libfsapfs_la_SOURCES)

