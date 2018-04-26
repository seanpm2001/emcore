//
// Created by Jose Miguel de la Rosa Trevin on 2017-10-15.
//

#ifndef EM_CORE_METADATA_H
#define EM_CORE_METADATA_H


#include <string>

#include "em/base/type.h"
#include "em/base/object.h"
#include "em/base/string.h"


namespace em {


    /**
     * List of Columns that can be accessed by ID or NAME.
     * Indexes start at 0.
     */
    class ColumnMap
    {
    public:
        static const size_t NO_ID;
        static const size_t NO_INDEX;

        /**
         * Class defining the properties of a given column in a Row or Table.
         * Each column should have an integer ID and a string NAME (both of which
         * should be unique among a given set of Columns). Additionally, a Column
         * should have a Type and could have a description of its meaning.
        */
        class Column
        {
        public:
            /** Constructor of a Column given the ID */
            Column(size_t id, const std::string &name, const Type & type,
                   const std::string &description="");
            /** Constructor of a Column without ID */
            Column(const std::string &name, const Type & type,
                   const std::string &description="");

            /** Return the ID of this Column */
            size_t getId() const;

            /** Return the NAME of this Column */
            std::string getName() const;

            /** Return the Type of this Column */
            const Type & getType() const;

            /** Return the description of this Column */
            std::string getDescription() const;

            private:
                size_t id;
                std::string name;
                Type type;
                std::string descr = "";

            friend class ColumnMap;

            }; // class Column

        using ColumnVector = std::vector<Column>;

        /** Default empty constructor */
        ColumnMap();

        /** Construct a ColumnMap from a list of Columns */
        ColumnMap(std::initializer_list<Column> list);

        /** Destructor */
        ~ColumnMap();

        /** Return the number of columns */
        size_t getSize() const;

        /** Return true if there are no columns */
        bool isEmpty() const;

        /** Add a new column, return the column index.
         * If the given column has no ID, it will be set.
        */
        size_t addColumn(const Column &column);

        /** Return the column with this column ID. */
        const Column& getColumn(size_t columnId);

        /** Return the column with this NAME */
        const Column& getColumn(const std::string &columnName);

        /** Return the index of the column with this ID */
        size_t getIndex(size_t columnId);

        /** Return the index of the column with this NAME. */
        size_t getIndex(const std::string &columnName);

        /** Return the column at this position */
        const Column& operator[](size_t pos);

        /** Return how many columns are in the Index. */
        size_t size() const;

        using iterator = ColumnVector::iterator;
        using const_iterator = ColumnVector::const_iterator;

        const_iterator begin() const;
        const_iterator end() const;

    private:
        class Impl;
        Impl * impl;

        // JMRT: For now disallow assignment between ColumnMap
        // If needed, we should implement the copy constructor and = operator
        ColumnMap &operator=(const ColumnMap& other) = default;

    }; // class ColumnMap


    /**
     * Class to store several rows of data values.
     * Each Row will contain value objects that are mapped to the Columns
     * defined in this Table.
     */
    class Table
    {
    private:
        /** Implementation class for Row and Table to use the PIMPL idiom */
        class RowImpl;
        class Impl;
        Impl * impl = nullptr;

    public:
        /**
         * Class to hold key-value pairs. Keys will be either string or integer
         * representing a given column. Values are instances of class Object
         * that can hold values of different types. The Type of a given object
         * in the Row should be of the same type of the Column's Type.
         */
        class Row
        {
        public:
            /** Empty constructor */
            Row() = default;

            /** Copy constructor and assignment */
            Row(const Row& other);
            /** Move constructor */
            Row(Row&& other) noexcept;

            /** Row Dtor */
            ~Row();

            /** Return the index of the column specified by columnId. */
            const Object& operator[](size_t columnId) const;
            Object& operator[](size_t columnId);

            /** Return the index of the column specified by columnName. */
            const Object& operator[](const std::string &columnName) const;
            Object& operator[](const std::string &columnName);


            Row& operator=(const Row& other);
            Row& operator=(Row&& other) noexcept;

            // FIXME: Not sure if this method should be placed here
            // there is not a single way to push to stream a Row
            void toStream(std::ostream &ostream) const;

            // TODO: Hide implementation details and implement a proper iterator
            using iterator = std::vector<Object>::iterator;
            using const_iterator = std::vector<Object>::const_iterator;

            iterator begin();
            iterator end();

        private:
            RowImpl * impl = nullptr;
            /** Construction of a Row, given its implementation.
             * Only accesible by Table.
             */
             Row(RowImpl * rowImpl);

            friend class Table;
        }; // class Row

        /** Empty Table constructor */
        Table();

        /** Table constructor based on input columns */
        Table(std::initializer_list<ColumnMap::Column> list);

        /** Destructor for Table */
        virtual ~Table();

        /** Clear all columns and rows */
        void clear();


        // ---------------- Column related methods ------------------------

        /** Return the row at this position */
        const Row& operator[](size_t pos) const;
        Row& operator[](size_t pos);

        /** Add a new column */
        void addColumn(const ColumnMap::Column &col);

        /** Insert a new column at a given position */
        void insertColumn(const ColumnMap::Column &col, size_t pos);

        /** Remove an existing column from the Table */
        void removeColumn(size_t colId);
        void removeColumn(const std::string &colName);

        /** Change the position of a column */
        void moveColumn(size_t colId, size_t pos);
        void moveColumn(const std::string &colName, size_t pos);

        /** Give access to a const object of the ColumnMap */
        const ColumnMap& getColumnMap() const;


        // ---------------- Row related methods ------------------------

        /** Return the number of rows in the Table */
        size_t getSize() const;

        /** Return true if the number of rows is 0 */
        bool isEmpty() const;

        /** Create a new row with the columns defined in this Table. */
        Row createRow() const;

        /** Add a new row to the set */
        bool addRow(const Row &row);

        /** Insert a new row in a given position */
        bool insertRow(const Row &row, size_t pos);

        /** Delete a given row */
        bool deleteRow(const Row &row);

        /** Delete all rows that match a query string
         * @returns The number of deleted rows.
         */
        size_t deleteRows(const std::string &queryStr);

        /** Update a given row */
        bool updateRow(const Row &row);

        /** Update many rows at once. Apply the operation string to all
         * rows that match the query string.
         * @returns The number of updated rows.
         */
        size_t updateRows(const std::string &operation,
                          const std::string &queryStr);

        // ---------------- Type definitions ------------------------
        using RowVector = std::vector<Row>;
        using iterator = RowVector::iterator;
        using const_iterator = RowVector::const_iterator;

        iterator begin();
        iterator end();

    }; // class Table

    /** @ingroup image
     * Read and write metadata (as table) from/to files.
     *
     * Internally, the TableIO class holds a pointer to TableIOImpl class,
     * that contains the details about how to open files and read the metadata.
     * The TableIOImpl class should be extended to provide support for other
     * formats.
     */
    class TableIO
    {
    public:
        class Impl;

        /** Used when registering new Impl classes.
         * The ImplBuilder is a function that should return a pointer to
         * a newly created implementation.
         */
        using ImplBuilder = Impl* (*)();

        /** Constants for open files. */
        static const int READ_ONLY = 0;
        static const int READ_WRITE = 1;
        static const int TRUNCATE = 2;

        /**
         * Empty constructor for TableIO.
         * In this case the newly created instance will have no format
         * implementation associated to read/write formats. Then, when the
         * open() method is called to open a file, the format implementation
         * will be inferred from the filename extension. Some functions will
         * raise an exception if called without having opened a file and,
         * therefore, without having an underlying format implementation.
         */
        TableIO();

        /**
         * Constructor to build a new TableIO instance given its name or
         * an extension related to the format implementation. The provided
         * input string should be the key associated to a know format
         * implementation. If not, an exception will be thrown. If the format
         * implementation is associated to the TableIO instance, it will not
         * change when calling the open() method. This allow to read/write
         * metadata with unknown (or non-standard) file extensions.
         *
         * @param extOrName Input string representing either the TableIO name
         * or one of the extensions registered for it.
         */
        TableIO(const std::string &extOrName);

        ~TableIO();

        /**
         * Check if some TableIO implementation is registered for a given name
         * or extension.
         *
         * @param extOrName Input string representing either the TableIO name
         * or one of the extensions registered.
         * @return Return True if there is any TableIO registered.
         */
        static bool hasImpl(const std::string &extOrName);

        /**
         * Register a new TableIO implementation.
         * This function should not be used unless you are developing an
         * implementation for a new TableIO format.
         */
        static bool registerImpl(const StringVector &extOrNames,
                                 ImplBuilder builder);

        // TODO: DOCUMENT
        void open(const std::string &path); //, const FileMode mode=READ_ONLY);
        // TODO: DOCUMENT
        void close();

        // TODO: DOCUMENT
        void read(const std::string &tableName, Table &table);

        // TODO: DOCUMENT
        void write(const std::string &tableName, const Table &table);

        // String representation
        void toStream(std::ostream &ostream) const;

    private:
        // Pointer to implementation class, PIMPL idiom
        Impl* impl = nullptr;
    }; // class TableIO


} // namespace em

std::ostream& operator<< (std::ostream &ostream, const em::Table::Row &row);


#endif //EM_CORE_METADATA_H
