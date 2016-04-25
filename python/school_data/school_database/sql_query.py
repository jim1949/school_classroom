#!/usr/bin/env python

class SqlQuery(object):

    def __init__(self):
        pass

    def add_num_s(self,sql_query,s_num):
        query = sql_query
        for i in range(0,s_num):
            query += '%s'
            if i != s_num - 1:
                query += ','
        return query

    def add_column_names(self,sql_query,columns_key):
        query = sql_query
        for i in range(0,len(columns_key)):
            query += columns_key[i]
            if i!=len(columns_key)-1:
                query += ','
        return  query

    def add_equal_expression(self,sql_query , data):
        query = sql_query
        judge = '%s = %s'
        for i in range(0,len(data)):
            query += judge%(data.keys()[i],'%s')
            if i != len(data) - 1:
                query += ' and '
        return query

    def get_insert_query(self,table_name,column_names):
        sql_query = 'insert into %s '%table_name
        sql_query += ' ('
        sql_query = self.add_column_names(sql_query,column_names)
        sql_query += ')'
        sql_query += ' values'
        sql_query += ' ('
        sql_query = self.add_num_s(sql_query,len(column_names))
        sql_query += ')'
        return sql_query


    def get_delete_query(self,table_name,delete_data_limit):
        sql_query = 'delete from %s'%table_name
        sql_query += ' where '
        sql_query = self.add_equal_expression(sql_query,delete_data_limit)
        return sql_query

    def get_update_query(self,table_name,update_data,update_data_limit):
        sql_query = 'update %s'%table_name
        sql_query += ' set '
        sql_query = self.add_equal_expression(sql_query,update_data)
        sql_query += ' where '
        sql_query = self.add_equal_expression(sql_query,update_data_limit)
        return sql_query


    def get_select_query(self,table_name,columns_names , limit_data):
        sql_query  = 'select '
        sql_query = self.add_column_names(sql_query,columns_names)
        sql_query += ' from %s where '%table_name
        sql_query = self.add_equal_expression(sql_query,limit_data)
        return sql_query




