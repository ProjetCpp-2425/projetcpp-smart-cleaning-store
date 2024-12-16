CREATE TABLE Employe (
    IDE INT PRIMARY KEY,         -- Employee ID, must be unique and positive
    CINE VARCHAR(8) NOT NULL,    -- CIN, 8-digit number
    NUME VARCHAR(8) NOT NULL,    -- Phone number, 8-digit number
    NOME VARCHAR(50) NOT NULL,   -- Last name of the employee
    PRENOME VARCHAR(50) NOT NULL,-- First name of the employee
    MAILE VARCHAR(100) NOT NULL, -- Email of the employee
    POSTEE VARCHAR(50) NOT NULL, -- Position of the employee
    DATE_EMBE DATE NOT NULL,     -- Date of employment
    STATUTE VARCHAR(20) NOT NULL,-- Status of the employee
    GENREE VARCHAR(10) NOT NULL, -- Gender of the employee
    AGEEE INT NOT NULL,          -- Age of the employee
    SALAIREE INT NOT NULL        -- Salary of the employee
);
