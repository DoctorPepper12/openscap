/*
 * Copyright 2020 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Authors:
 *      Jan Černý <jcerny@redhat.com>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "yamlfilecontent_probe.h"
#include "sexp-manip.h"
#include "debug_priv.h"
#include "oval_fts.h"

static int process_yaml_file(const char *path, const char *filename, const char *yamlpath, probe_ctx *ctx)
{
	char *filepath = oscap_path_join(path, filename);

	/* TODO: insert code using libyaml-yamlpath-filter here */

	SEXP_t *item = probe_item_create(
		OVAL_INDEPENDENT_YAML_FILE_CONTENT,
		NULL,
		"filepath", OVAL_DATATYPE_STRING, filepath,
		"path", OVAL_DATATYPE_STRING, path,
		"filename", OVAL_DATATYPE_STRING, filename,
		"yamlpath", OVAL_DATATYPE_STRING, yamlpath,
		/*
		"value_of",
		"windows_view",
		*/
		NULL
	);
	probe_item_collect(ctx, item);
	free(filepath);
	return 0;
}

int yamlfilecontent_probe_main(probe_ctx *ctx, void *arg)
{
	SEXP_t *probe_in = probe_ctx_getobject(ctx);
	SEXP_t *behaviors_ent = probe_obj_getent(probe_in, "behaviors", 1);
	SEXP_t *filepath_ent = probe_obj_getent(probe_in, "filepath", 1);
	SEXP_t *path_ent = probe_obj_getent(probe_in, "path", 1);
	SEXP_t *filename_ent = probe_obj_getent(probe_in, "filename", 1);
	SEXP_t *yamlpath_ent = probe_obj_getent(probe_in, "yamlpath", 1);
	SEXP_t *yamlpath_val = probe_ent_getval(yamlpath_ent);
	char *yamlpath_str = SEXP_string_cstr(yamlpath_val);

	probe_filebehaviors_canonicalize(&behaviors_ent);
	OVAL_FTS *ofts = oval_fts_open_prefixed(
		NULL, path_ent, filename_ent, filepath_ent, behaviors_ent,
		probe_ctx_getresult(ctx));
	if (ofts != NULL) {
		OVAL_FTSENT *ofts_ent;
		while ((ofts_ent = oval_fts_read(ofts)) != NULL) {
			if (ofts_ent->fts_info == FTS_F
			    || ofts_ent->fts_info == FTS_SL) {
				process_yaml_file(
					ofts_ent->path, ofts_ent->file, yamlpath_str, ctx);
			}
			oval_ftsent_free(ofts_ent);
		}
		oval_fts_close(ofts);
	}

cleanup:
	free(yamlpath_str);
	SEXP_free(yamlpath_val);
	SEXP_free(yamlpath_ent);
	SEXP_free(filename_ent);
	SEXP_free(path_ent);
	SEXP_free(filepath_ent);
	SEXP_free(behaviors_ent);
	return 0;
}
