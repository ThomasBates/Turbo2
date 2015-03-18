using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Turbo.Budget.Types;

namespace Turbo.Budget.SQLCE
{
    class TurboBudgetTransaction : ITurboBudgetTransaction
    {
        public int TransactionId { get; set; }
        public int Order { get; set; }
        public DateTime TimeStamp { get; set; }
        public int FromAccountId { get; set; }
        public ITurboBudgetAccount FromAccount { get; set; }
        public int ToAccountId { get; set; }
        public ITurboBudgetAccount ToAccount { get; set; }
        public decimal Amount { get; set; }
        public string Description { get; set; }
    }
}
