# Database Management System (DBMS) Guide

## Introduction

In this guide, you learn how to use the DBMS.

## Guide

### 1. Commands to the database

#### Create

```shell
create database "database"
```

#### Use

```shell
use "database"
```

### 2. Commands to the table

#### Create

```shell
create table "table"
```

#### Add columns

```shell
alter table "table" add (column value size)
```

#### Describe

```shell
describe "table"
```

#### Add register

```shell
insert into "table" values (information,...)
```

#### Pass csv

```shell
load data "archive.csv" into table "table"
```

### 3. Commands to view information

#### All

```shell
select * from "table"
```

#### Specific

```shell
select (column(s),...) from "table"
```

#### With conditional

```shell
select * from "table" where (column operator value)
```

#### or

```shell
select (column(s),...) from "table" where (column operator value)
```

### 4. Commands to create a table with information

```shell
select * from "table" | "newtable"
```

#### or

```shell
select (column(s),...) from "table" | "newtable"
```

#### or

```shell
select * from "table" where (column operator value) | "newtable"
```

#### or

```shell
select (column(s),...) from "table" where (column operator value) | "newtable"
```

### 5. Others commands

```shell
exit
```

#### or

```shell
clear
```
