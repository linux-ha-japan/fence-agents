/*
  Copyright Red Hat, Inc. 2005

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 2, or (at your option) any
  later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; see the file COPYING.  If not, write to the
  Free Software Foundation, Inc.,  675 Mass Ave, Cambridge, 
  MA 02139, USA.
*/
/*
 * Author: Stanko Kupcevic <kupcevic@redhat.com>
 */
/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.iterate_access.conf,v 1.6 2003/09/29 22:30:36 hardaker Exp $
 */


#include "nodesMIB.h"
#include "nodesMIB_checkfns.h"
#include "nodesMIB_access.h"


void
initialize_nodesMIB(void)
{
  init_rhcNodesTable();
}



static netsnmp_oid_stash_node *undoStorage = NULL;
static netsnmp_oid_stash_node *commitStorage = NULL;

struct undoInfo {
    void           *ptr;
    size_t          len;
};

struct commitInfo {
    void           *data_context;
    int             have_committed;
    int             new_row;
};


void
rhcNodesTable_free_undoInfo(void *vptr)
{
  struct undoInfo *ui = (struct undoInfo*) vptr;
  if (!ui)
    return;
  SNMP_FREE(ui->ptr);
  SNMP_FREE(ui);
}

/** Initialize the rhcNodesTable table by defining its contents and how it's structured */
void
initialize_table_rhcNodesTable(void)
{
    static oid      rhcNodesTable_oid[] =
        { 1, 3, 6, 1, 4, 1, 2312, 8, 3, 1 };
    netsnmp_table_registration_info *table_info;
    netsnmp_handler_registration *my_handler;
    netsnmp_iterator_info *iinfo;

    /** create the table registration information structures */
    table_info = SNMP_MALLOC_TYPEDEF(netsnmp_table_registration_info);
    iinfo = SNMP_MALLOC_TYPEDEF(netsnmp_iterator_info);

    /** if your table is read only, it's easiest to change the
        HANDLER_CAN_RWRITE definition below to HANDLER_CAN_RONLY */
    my_handler = netsnmp_create_handler_registration("rhcNodesTable",
                                                     rhcNodesTable_handler,
                                                     rhcNodesTable_oid,
                                                     OID_LENGTH
                                                     (rhcNodesTable_oid),
                                                     HANDLER_CAN_RONLY);
    
    if (!my_handler || !table_info || !iinfo) {
        snmp_log(LOG_ERR,
                 "malloc failed in initialize_table_rhcNodesTable");
        return; /** Serious error. */
    }

    /***************************************************
     * Setting up the table's definition
     */
    netsnmp_table_helper_add_indexes(table_info, ASN_OCTET_STR,
				     /** index: rhcNodeName */
                                     0);

    /** Define the minimum and maximum accessible columns.  This
        optimizes retrival. */
    table_info->min_column = 1;
    table_info->max_column = 5;

    /** iterator access routines */
    iinfo->get_first_data_point = rhcNodesTable_get_first_data_point;
    iinfo->get_next_data_point = rhcNodesTable_get_next_data_point;

    /** you may wish to set these as well */
    //#ifdef MAYBE_USE_THESE
    iinfo->make_data_context = rhcNodesTable_context_convert_function;
    iinfo->free_data_context = rhcNodesTable_data_free;
    
    /** pick *only* one of these if you use them */
    //iinfo->free_loop_context = rhcNodesTable_loop_free;
    iinfo->free_loop_context_at_end = rhcNodesTable_loop_free;
    //#endif

    /** tie the two structures together */
    iinfo->table_reginfo = table_info;

    /***************************************************
     * registering the table with the master agent
     */
    DEBUGMSGTL(("initialize_table_rhcNodesTable",
                "Registering table rhcNodesTable as a table iterator\n"));
    netsnmp_register_table_iterator(my_handler, iinfo);
}

/** Initializes the rhcNodesTable module */
void
init_rhcNodesTable(void)
{

  /** here we initialize all the tables we're planning on supporting */
    initialize_table_rhcNodesTable();
}

/** handles requests for the rhcNodesTable table, if anything else needs to be done */
int
rhcNodesTable_handler(netsnmp_mib_handler *handler,
                      netsnmp_handler_registration *reginfo,
                      netsnmp_agent_request_info *reqinfo,
                      netsnmp_request_info *requests)
{

    netsnmp_request_info *request;
    netsnmp_table_request_info *table_info;
    netsnmp_variable_list *var;
    struct commitInfo *ci = NULL;

    void           *data_context = NULL;

    oid            *suffix;
    size_t          suffix_len;

    /** column and row index encoded portion */
    suffix = requests->requestvb->name + reginfo->rootoid_len + 1;
    suffix_len = requests->requestvb->name_length -
        (reginfo->rootoid_len + 1);

    for (request = requests; request; request = request->next) {
        var = request->requestvb;
        if (request->processed != 0)
            continue;

        switch (reqinfo->mode) {
        case MODE_GET:
        case MODE_SET_RESERVE1:
            data_context = netsnmp_extract_iterator_context(request);
            if (data_context == NULL) {
                if (reqinfo->mode == MODE_GET) {
                    netsnmp_set_request_error(reqinfo, request,
                                              SNMP_NOSUCHINSTANCE);
                    continue;
                }
            }
            break;

        default:               /* == the other SET modes */
            ci = (struct commitInfo*) netsnmp_oid_stash_get_data(commitStorage,
								 suffix + 1, suffix_len - 1);
            break;

        }

        /** extracts the information about the table from the request */
        table_info = netsnmp_extract_table_info(request);
        /** table_info->colnum contains the column number requested */
        /** table_info->indexes contains a linked list of snmp variable
           bindings for the indexes of the table.  Values in the list
           have been set corresponding to the indexes of the
           request */
        if (table_info == NULL) {
            continue;
        }

        switch (reqinfo->mode) {
        case MODE_GET:
            switch (table_info->colnum) {
            case COLUMN_RHCNODENAME:
                {
                    char           *retval;
                    size_t          retval_len = 0;
                    retval = get_rhcNodeName(data_context, &retval_len);
                    snmp_set_var_typed_value(var, ASN_OCTET_STR,
                                             (const u_char *) retval,
                                             retval_len);
                }
                break;

            case COLUMN_RHCNODESTATUSCODE:
                {
                    long           *retval;
                    size_t          retval_len = 0;
                    retval =
                        get_rhcNodeStatusCode(data_context, &retval_len);
                    snmp_set_var_typed_value(var, ASN_INTEGER,
                                             (const u_char *) retval,
                                             retval_len);
                }
                break;

            case COLUMN_RHCNODESTATUSDESC:
                {
                    char           *retval;
                    size_t          retval_len = 0;
                    retval =
                        get_rhcNodeStatusDesc(data_context, &retval_len);
                    snmp_set_var_typed_value(var, ASN_OCTET_STR,
                                             (const u_char *) retval,
                                             retval_len);
                }
                break;

            case COLUMN_RHCNODERUNNINGSERVICESNUM:
                {
                    long           *retval;
                    size_t          retval_len = 0;
                    retval =
                        get_rhcNodeRunningServicesNum(data_context,
                                                      &retval_len);
                    snmp_set_var_typed_value(var, ASN_INTEGER,
                                             (const u_char *) retval,
                                             retval_len);
                }
                break;

            case COLUMN_RHCNODERUNNINGSERVICESNAMES:
                {
                    char           *retval;
                    size_t          retval_len = 0;
                    retval =
                        get_rhcNodeRunningServicesNames(data_context, 
							&retval_len);
                    snmp_set_var_typed_value(var, ASN_OCTET_STR,
                                             (const u_char *) retval,
                                             retval_len);
                }
                break;

            default:
                /** We shouldn't get here */
                snmp_log(LOG_ERR,
                         "problem encountered in rhcNodesTable_handler: unknown column\n");
            }
            break;

        case MODE_SET_RESERVE1:
            ci = (struct commitInfo*) netsnmp_oid_stash_get_data(commitStorage,
								 suffix + 1, suffix_len - 1);

            if (!ci) {
                    /** create the commit storage info */
                ci = SNMP_MALLOC_STRUCT(commitInfo);
                if (!data_context) {
                    ci->data_context =
                        rhcNodesTable_create_data_context(table_info->
                                                          indexes);
                    ci->new_row = 1;
                } else {
                    ci->data_context = data_context;
                }
                netsnmp_oid_stash_add_data(&commitStorage,
                                           suffix + 1, suffix_len - 1, ci);
            }
            break;

        case MODE_SET_RESERVE2:
            switch (table_info->colnum) {
            default:
                netsnmp_set_request_error(reqinfo, request,
                                          SNMP_ERR_NOTWRITABLE);
                break;
            }
            break;

        case MODE_SET_ACTION:
            /** save a variable copy */
            switch (table_info->colnum) {
            }
            break;

        case MODE_SET_COMMIT:
            if (!ci->have_committed) {
                    /** do this once per row only */
                rhcNodesTable_commit_row(&ci->data_context, ci->new_row);
                ci->have_committed = 1;
            }
            break;

        case MODE_SET_UNDO:
             /** save a variable copy */
            switch (table_info->colnum) {
            }
            break;

        case MODE_SET_FREE:
            break;

        default:
            snmp_log(LOG_ERR,
                     "problem encountered in rhcNodesTable_handler: unsupported mode\n");
        }
    }

    /** clean up after all requset processing has ended */
    switch (reqinfo->mode) {
    case MODE_SET_UNDO:
    case MODE_SET_FREE:
    case MODE_SET_COMMIT:
        /** clear out the undo cache */
        netsnmp_oid_stash_free(&undoStorage, rhcNodesTable_free_undoInfo);
        netsnmp_oid_stash_free(&commitStorage, netsnmp_oid_stash_no_free);
    }


    return SNMP_ERR_NOERROR;
}
