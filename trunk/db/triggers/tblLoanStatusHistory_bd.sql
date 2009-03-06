DELIMITER $$
DROP TRIGGER IF EXISTS tblLoanStatusHistory_bd$$
CREATE TRIGGER tblLoanStatusHistory_bd BEFORE DELETE ON tblLoanStatusHistory FOR EACH ROW
BEGIN
  INSERT INTO tblLoanStatusHistoryTrash (id,loan_id,p_status,status,date,time,user_id,memo) VALUES (OLD.id,OLD.loan_id,OLD.p_status,OLD.status,OLD.date,OLD.time,OLD.user_id,OLD.memo);
END$$
