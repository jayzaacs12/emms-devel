DELIMITER $$
DROP TRIGGER IF EXISTS tblLoansMasterDetails_bd$$
CREATE TRIGGER tblLoansMasterDetails_bd BEFORE DELETE ON tblLoansMasterDetails FOR EACH ROW
BEGIN
  INSERT INTO tblLoansMasterDetailsTrash (master_id,loan_id) VALUES (OLD.master_id,OLD.loan_id);
END$$
