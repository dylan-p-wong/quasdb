#pragma once

enum class StatementType
{
    // Begin,
    // Commit,
    // Rollback,
    // Explain,
    CreateTable,
    DropTable,
    Delete,
    Insert,
    Update,
    Select
};

class Statement {
public:
    StatementType type;
    Statement(StatementType type);
    virtual ~Statement() = 0;
};
