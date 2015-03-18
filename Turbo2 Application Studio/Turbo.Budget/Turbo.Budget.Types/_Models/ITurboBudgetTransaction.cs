using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Turbo.Budget.Types
{
    public interface ITurboBudgetTransaction
    {
        int TransactionId { get; set; }
        int Order { get; set; }
        DateTime TimeStamp { get; set; }
        int FromAccountId { get; set; }
        ITurboBudgetAccount FromAccount { get; set; }
        int ToAccountId { get; set; }
        ITurboBudgetAccount ToAccount { get; set; }
        decimal Amount { get; set; }
        string Description { get; set; }
    }
}
