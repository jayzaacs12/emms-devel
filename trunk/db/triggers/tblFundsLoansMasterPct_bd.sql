DELIMITER $$
DROP TRIGGER IF EXISTS tblFundsLoansMasterPct_bd$$
CREATE TRIGGER tblFundsLoansMasterPct_bd BEFORE DELETE ON tblFundsLoansMasterPct FOR EACH ROW
BEGIN
  INSERT INTO tblFundsLoansMasterPctTrash (id,master_id,fund_id,pct) VALUES (OLD.id,OLD.master_id,OLD.fund_id,OLD.pct);
END$$
