CREATE TABLE CLIENT (
    CIN VARCHAR2(20) PRIMARY KEY,
    ID NUMBER(10) NOT NULL,
    NOM VARCHAR2(100),
    NUM VARCHAR2(20),
    DATE_NAIS DATE,
    ADRESSE VARCHAR2(255)
);