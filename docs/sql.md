# SQL

## Data Types

Types that are supported for storing in the database. They are stored in an AbstractData class with a templated Data.

| Type ||
| ----------- | ----------- |
| Boolean | true or false |
| Integer | standard integer |
| Float | standard float |
| Varchar | string |
| Null | special |


## Operators

| Infix Math Operations ||
| ----------- | ----------- |
| + | addition |
| - | subtraction |
| * | multiplication |
| / | division |
| ^ | exponentiation |
| % | modulo |

| Prefix Math Operations ||
| ----------- | ----------- |
| + | assert |
| - | negate |

| Infix Logical Operations ||
| ----------- | ----------- |
| AND | and |
| OR | or |

| Prefix Logical Operations ||
| ----------- | ----------- |
| NOT | not |

| Infix Comparision Operations ||
| ----------- | ----------- |
| < | less than |
| > | greater than |
| = | equality |
| != | inequality |

## Statements
### Create Table
Examples
```
CREATE TABLE TEST (x integer, y float, z varchar(255))
```

---
### Drop Table
Examples
```
DROP TABLE TEST
```
---
### Insert
Examples
```
INSERT INTO TEST VALUES (1 * 8, 7.79, 'nas')
```
```
INSERT INTO TEST (x, z) VALUES (1 * 8, 'nas')
```
---
### Delete
Examples
```
DELETE TEST
```
```
DELETE TEST WHERE TEST.id = 5
```
---
### Update
Examples
```
UPDATE TEST SET x=5
```
```
UPDATE TEST SET x=5, y=7 WHERE TEST.id = 1
```
---
### Select

| Join | Supported |
| ----------- | ----------- |
| INNER | ✅ |
| LEFT | 🏗️ |
| RIGHT | 🏗️ |
| OUTER | 🏗️ |

| Keyword | Supported |
| ----------- | ----------- |
| WHERE | ✅ |
| GROUP BY | 🏗️ |
| HAVING | 🏗️ |
| ORDER BY | 🏗️ |
| OFFSET | ✅ |
| LIMIT | ✅ |

Examples
```
SELECT * FROM TEST WHERE TEST.x = 5
```
```
SELECT * FROM TEST1, TEST2
```
```
SELECT * FROM TEST1 JOIN TEST2 on TEST1.id=TEST2.id
```
---
## Transactions

In Progress 🏗️
