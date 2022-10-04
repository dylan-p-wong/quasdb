#include <algorithm>
#include <vector>

#include "catalog.h"

Catalog::Catalog(BufferManager * buffer_manager) : buffer_manager{buffer_manager} {
    LoadStartingTables();
    LoadTables();
}

CatalogTable * Catalog::InitTableInfoTable() {

    std::vector<CatalogColumn*> columns;

    int current_tuple_size = 0;

    Column table_name_column{"table_name"};
    table_name_column.data_type = DataType::Varchar;
    table_name_column.nullable = false;
    table_name_column.size = 64;
    CatalogColumn * c1 = new CatalogColumn{table_name_column, current_tuple_size};
    columns.emplace_back(c1);
    current_tuple_size += c1->GetColumnSize();

    Column first_column_page_id_column{"tuple_size"};
    first_column_page_id_column.data_type = DataType::Integer;
    first_column_page_id_column.nullable = false;
    CatalogColumn * c2 = new CatalogColumn{first_column_page_id_column, current_tuple_size};
    columns.emplace_back(c2);
    current_tuple_size += c2->GetColumnSize();

    Column first_data_page_id_column{"first_data_page_id"};
    first_data_page_id_column.data_type = DataType::Integer;
    first_data_page_id_column.nullable = false;
    CatalogColumn * c3 = new CatalogColumn{first_data_page_id_column, current_tuple_size};
    columns.emplace_back(c3);
    current_tuple_size += c3->GetColumnSize();

    Column number_of_columns_column{"number_of_columns"};
    number_of_columns_column.data_type = DataType::Integer;
    number_of_columns_column.nullable = false;
    CatalogColumn * c4 = new CatalogColumn{number_of_columns_column, current_tuple_size};
    columns.emplace_back(c4);
    current_tuple_size += c4->GetColumnSize();

    CatalogTable * ct = new CatalogTable{"table_info", columns, current_tuple_size};

    ct->SetFirstDirectoryPage(0);

    return ct;
}

CatalogTable * Catalog::InitColumnInfoTable() {
    std::vector<CatalogColumn*> columns;

    Column table_name_column{"table_name"};
    table_name_column.data_type = DataType::Varchar;
    table_name_column.nullable = false;
    table_name_column.size = 64;
    CatalogColumn * c1 = new CatalogColumn{table_name_column, 0};
    columns.emplace_back(c1);

    Column column_name_column{"column_name"};
    column_name_column.data_type = DataType::Varchar;
    column_name_column.nullable = false;
    column_name_column.size = 64;
    CatalogColumn * c2 = new CatalogColumn{column_name_column, c1->GetColumnSize()};
    columns.emplace_back(c2);

    Column data_type_column{"data_type"};
    data_type_column.data_type = DataType::Integer;
    data_type_column.nullable = false;
    CatalogColumn * c3 = new CatalogColumn{data_type_column, c1->GetColumnSize() + c2->GetColumnSize()};
    columns.emplace_back(c3);

    Column nullable_column{"nullable"};
    nullable_column.data_type = DataType::Boolean;
    nullable_column.nullable = false;
    CatalogColumn * c4 = new CatalogColumn{nullable_column, c1->GetColumnSize() + c2->GetColumnSize() + c3->GetColumnSize()};
    columns.emplace_back(c4);

    Column offset_column{"offset"};
    offset_column.data_type = DataType::Integer;
    offset_column.nullable = false;
    CatalogColumn * c5 = new CatalogColumn{offset_column, c1->GetColumnSize() + c2->GetColumnSize() + c3->GetColumnSize() + c4->GetColumnSize()};
    columns.emplace_back(c5);

    Column column_size_column{"column_size"};
    column_size_column.data_type = DataType::Integer;
    column_size_column.nullable = false;
    CatalogColumn * c6 = new CatalogColumn{column_size_column,  c1->GetColumnSize() + c2->GetColumnSize() + c3->GetColumnSize() + c4->GetColumnSize() + c5->GetColumnSize()};
    columns.emplace_back(c6);

    CatalogTable * ct = new CatalogTable{"column_info", columns, c1->GetColumnSize() + c2->GetColumnSize() + c3->GetColumnSize() + c4->GetColumnSize() + c5->GetColumnSize() + c6->GetColumnSize()};

    ct->SetFirstDirectoryPage(1);

    return ct;
}

Result<void, Error> Catalog::CreateTable(CatalogTable * table) {
    // Validate table
    if (table->ValidateTable() == false) {
        return Err(Error{ErrorType::Internal, "Bad table."});
    }

    // Duplicate table name
    for (CatalogTable * existing_table : tables) {
        if (existing_table->GetTableName() == table->GetTableName()) {
            return Err(Error{ErrorType::Internal, "Table with name already exists."});
        }
    }

    // Bad table column references
    for (std::pair<std::string, std::string> reference : table->GetReferences()) {
        bool found = false;

        for (CatalogTable * existing_table : tables) {
            if (existing_table->GetTableName() == reference.first) {
                for (CatalogColumn * column : existing_table->GetColumns()) {
                    if (column->GetColumnName() == reference.second) {
                        found = true;
                    }
                }
            }
        }

        if (!found) {
            return Err(Error{ErrorType::Internal, "Bad table column reference."});
        }
    }

    table->SetFirstDirectoryPage(buffer_manager->NewPage()->GetPageId());
    DirectoryPage * p = reinterpret_cast<DirectoryPage*>(buffer_manager->GetPage(table->GetFirstDataPageDirectoryPageId()));
    p->Init();

    if (table_info_table && column_info_table) { // TEST
        InsertIntoTableInfoTable(table);
        InsertInfoColumnInfoTable(table);
    }

    tables.push_back(table);
    return Ok();
}

Result<void, Error> Catalog::InsertInfoColumnInfoTable(CatalogTable * table) {
    
    for (CatalogColumn * column : table->GetColumns()) {
        std::vector<std::unique_ptr<AbstractData>> t_values;
        
        t_values.emplace_back(std::make_unique<Data<std::string>>(DataType::Varchar, table->GetTableName()));
        t_values.emplace_back(std::make_unique<Data<std::string>>(DataType::Varchar, column->GetColumnName()));
        t_values.emplace_back(std::make_unique<Data<int>>(DataType::Integer, DataTypeToInt(column->GetColumnDataType())));
        t_values.emplace_back(std::make_unique<Data<bool>>(DataType::Boolean, column->GetNullable()));
        t_values.emplace_back(std::make_unique<Data<int>>(DataType::Integer, column->GetColumnOffset()));
        t_values.emplace_back(std::make_unique<Data<int>>(DataType::Integer, column->GetColumnSize()));

        if (!column_info_table->ValidateRow(t_values)) {
            throw Error{ErrorType::Internal, "InsertInfoColumnInfoTable Error"};
        }

        const InputTuple t{t_values, column_info_table};
        column_info_table->InsertTuple(t, buffer_manager);
    }

    return Ok();
}

Result<void, Error> Catalog::InsertIntoTableInfoTable(CatalogTable * table) {
    std::vector<std::unique_ptr<AbstractData>> t_values;
    t_values.emplace_back(std::make_unique<Data<std::string>>(DataType::Varchar, table->GetTableName()));
    t_values.emplace_back(std::make_unique<Data<int>>(DataType::Integer, table->GetLengthOfTuple())); // not used atm
    t_values.emplace_back(std::make_unique<Data<int>>(DataType::Integer, table->GetFirstDataPageDirectoryPageId())); // not used atm
    t_values.emplace_back(std::make_unique<Data<int>>(DataType::Integer, table->GetNumberOfColumns())); // not used atm

    if (!table_info_table->ValidateRow(t_values)) {
        throw Error{ErrorType::Internal, "InsertIntoTableInfoTable Error"};
    }

    const InputTuple t{t_values, table_info_table};
    return table_info_table->InsertTuple(t, buffer_manager);
}

Result<void, Error> Catalog::DeleteTable(const std::string & table_name) {
    for (int i = 0; i < tables.size(); i++) {
        CatalogTable * existing_table = tables.at(i);

        if (existing_table->GetTableName() == table_name) {
            tables.erase(tables.begin() + i);
            return Ok();
        }
    }

    return Err(Error{ErrorType::Internal, "Table with name not found."});
}

Result<CatalogTable*, Error> Catalog::ReadTable(const std::string & table_name) {
    for (CatalogTable * existing_table : tables) {
        if (existing_table->GetTableName() == table_name) {
            return Ok(existing_table);
        }
    }

    if (table_name == "systeminfo") {
        return Ok(table_info_table);
    }

    if (table_name == "columninfo") {
        return Ok(column_info_table);
    }

    return Err(Error{ErrorType::Internal, "Table with name not found."});
}

void Catalog::LoadStartingTables() {    
    if (buffer_manager == nullptr) { // TESTING
        return;
    }

    table_info_table = InitTableInfoTable();
    
    if (table_info_table->ValidateTable() == false) {
        throw Error{ErrorType::Internal, "Bad table."};
    }

    try {
        buffer_manager->GetPage(0);
    } catch (Error & e) {
        DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->NewPage());
        dp->Init();
    }

    column_info_table = InitColumnInfoTable();

    if (column_info_table->ValidateTable() == false) {
        throw Error{ErrorType::Internal, "Bad table."};
    }

    try {
        buffer_manager->GetPage(1);
    } catch (Error & e) {
        DirectoryPage * dp = reinterpret_cast<DirectoryPage*>(buffer_manager->NewPage());
        dp->Init();
    }
}

void Catalog::LoadTables() {
    if (buffer_manager == nullptr) { // TESTING
        return;
    }

    std::vector<OutputTuple*> column_info_tuples = column_info_table->GetTuples(buffer_manager);
    std::vector<OutputTuple*> table_info_tuples = table_info_table->GetTuples(buffer_manager);

    for (OutputTuple * table_info_tuple : table_info_tuples) {

        std::vector<AbstractData*> table_info_values = table_info_tuple->GetAsValues(table_info_table);

        std::string table_name = dynamic_cast<const Data<std::string>*>(table_info_values.at(0))->value;
        int tuple_size = dynamic_cast<const Data<int>*>(table_info_values.at(1))->value;
        int first_directory_page_id = dynamic_cast<const Data<int>*>(table_info_values.at(2))->value;

        std::vector<CatalogColumn*> columns;

        for (OutputTuple * column_info_tuple : column_info_tuples) {
            std::vector<AbstractData*> column_info_values = column_info_tuple->GetAsValues(column_info_table);
            
            std::string column_table_name = dynamic_cast<const Data<std::string>*>(column_info_values.at(0))->value;

            if (column_table_name != table_name) {
                continue;
            }

            std::string column_name = dynamic_cast<const Data<std::string>*>(column_info_values.at(1))->value;

            Column c{column_name};

            int data_type_int = dynamic_cast<const Data<int>*>(column_info_values.at(2))->value;
            bool nullable = dynamic_cast<const Data<bool>*>(column_info_values.at(3))->value;
            int offset = dynamic_cast<const Data<int>*>(column_info_values.at(4))->value;
            int column_size = dynamic_cast<const Data<int>*>(column_info_values.at(5))->value;

            c.nullable = nullable;
            c.data_type = IntToDataType(data_type_int);
            c.size = column_size;

            CatalogColumn * cc = new CatalogColumn{c, offset};
            columns.emplace_back(cc);
        }

        CatalogTable * t = new CatalogTable{table_name, columns, tuple_size};
        t->SetFirstDirectoryPage(first_directory_page_id);
        tables.emplace_back(t);
    }
}
