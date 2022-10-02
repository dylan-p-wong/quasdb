#include <algorithm>
#include <vector>

#include "catalog.h"

Catalog::Catalog(BufferManager * buffer_manager) : buffer_manager{buffer_manager} {
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
    table_info_table->SetFirstDirectoryPage(0);

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
    column_info_table->SetFirstDirectoryPage(1);
}

CatalogTable * Catalog::InitTableInfoTable() {

    std::vector<CatalogColumn*> columns;

    int current_tuple_size = 0;

    Column table_name_column{"table_name"};
    table_name_column.data_type = DataType::Varchar;
    table_name_column.nullable = false;
    table_name_column.size = 16;
    CatalogColumn * c1 = new CatalogColumn{table_name_column, current_tuple_size};
    columns.emplace_back(c1);
    current_tuple_size += c1->GetColumnSize();

    Column first_column_page_id_column{"first_column_page_id"};
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

    return new CatalogTable{"table_info", columns, current_tuple_size};
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

    return new CatalogTable{"column_info", columns, c1->GetColumnSize() + c2->GetColumnSize() + c3->GetColumnSize() + c4->GetColumnSize() + c5->GetColumnSize()};
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

    InsertIntoTableInfoTable(table);
    InsertInfoColumnInfoTable(table);

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
    t_values.emplace_back(std::make_unique<Data<int>>(DataType::Integer, table->GetFirstDataPageDirectoryPageId())); // not used atm
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
